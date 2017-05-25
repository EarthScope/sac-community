#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "extfunc.h"

#include "amf.h"
#include "SacHeader.h"
#include "hdr.h"
#include "cpf.h"
#include "debug.h"
#include "dfm.h"
#include "msg.h"
#include "debug.h"

#define EPSILON 1e-5

/* ROTINC TO [VRT|LQT|VNE|XYZ] { ANGLE ang | iP | iS } { VP alpha VS beta RAY ray parameter }
 INPUT:
      TO VRT: rotate into vertical,radial, transverse coordinate system
      TO LQT: rotate into P,SV, and SH coordinate system.  Angle is apparent angle
      TO VNE,XYZ: rotate into system aligned with N, E and vertical
          XYZ still works, but is not mentioned in the HELP file.

      ANGLE: is chosen rotation angle around T from vertical (up) to L direction
      iP: incident P wave (need to set VP, RAY)  Now only with free-surface response
      iS: incident S wave (need to set VS, RAY)  Now only with free-surface response

      VP: P wave velocity near surface (default 5.8)
      VS: S wave velocity near surface (default 3.36)  These are iasp91 values
      RAY: ray parameter= horizontal slowness (s/km) (default unphysical -10.0)

      VERBOSE: Prints out details


 DESCRIPTION:
 Given three perpendicular components of a recorded time series, it rotates them
 into selected coordinate systems. It works its way through all records in memory
 until it fails to find 3 consecutive perpendicular traces. It assumes
 vertical up (cmpinc = 0.0), cmpaz measured clockwise from N.  VNE is a
 left-handed coordinate system.  With one's back to the epicenter, R is
 towards the station and T is to the right, so VRT is a left-handed
 coordinate system.  The routine does not explicitly check that the three components
 are perpendicular, but it probaly suffices that they are linearly independent.

 HEADER CHANGES:
 CMPINC,CMPAZ,KCMPNM (DEPMAX,DEPMIN,DEPMEN)

 This command was originally an external command written by Frederik Tilmann.
 Currently an internal command
*/
/* (C) 2000 Frederik Tilmann   Modified in May 2017 by Arthur Snoke */
/* Target systems */
enum CoordSystem {
    Unknown_CoordSystem = -1,
    XYZ     =  0,   /* Z: up X points towards E, Y points towards N */
    VNE     =  1,
    VRT     =  2,
    LQT     =  3,
};
const char *target_str[] = {
    "XYZ", "VNE", "VRT", "LQT",
};
/* Methods  */
enum LQT_Method {
    Unknown_Method = -1,
    ROTANG  =  0,
    IP      =  1,
    IS      =  2,
};
const char *imethod_str[] = {
    "ROTANG", "IP", "IS",
};
#define PI M_PI
#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

void
matrix_print(float **a, int n, int m) {
    int i,j;
    for(i = 0; i < n; i++) {
        printf("[ ");
        for(j = 0; j < m; j++) {
            printf("%8.5f ", a[i][j]);
        }
        printf(" ]\n");
    }
}

float **
matrix_mul(float **a, float **b, float **c, int n1, int n, int n2) {
    double sum;
    int i,j,k;
    for(i = 0; i < n1; i++) {
        for(j = 0; j < n2; j++) {
            sum = 0.0;
            for(k = 0; k < n; k++) {
                sum += a[i][k] * b[k][j];
            }
            c[i][j] = sum;
        }
    }
    return c;
}

float
dot(float *a, float *b, int n) {
    int i;
    double sum = 0.0;
    for(i = 0; i < n; i++) {
        sum += a[i] * b[i];
    }
    return (float) sum;
}


/* Internal prototypes */
void parse_rotinc(enum CoordSystem *target, double *angle, int *verbose, int *nerr);
int statimcmp(sac *a, sac *b);

void
rotinc(int *nerr) {
    int numfiles,i,j,k;
    float cmpinc,cmpaz;
    long npts;

    int target;
    int verbose = 0;
    double angle,si,ci;
    /* float **base=matrix(1,3,1,3); */
    /* float **dum=matrix(1,3,1,3); */
    /* float **Ltb=matrix(1,3,1,3),**Ltar=matrix(1,3,1,3);  */

    float **base;
    /* float **dum=alloc_fmatrix(3,3); */
    float **Ltb, **Ltar;
    float *traces[3],**newtraces ;
    float baz;

    sac *s[3];

    base = Ltb = Ltar = NULL;
    /* - Check for null data file list. */
    vflist(nerr);
    if (*nerr != 0) {
        goto L_8888;
    }
    /* - Check to make sure all files are evenly
       spaced time series files. */
    vfeven(nerr);
    if (*nerr != 0) {
        goto L_8888;
    }

    base = (float **) malloc(sizeof(float *) * 3);
    Ltb  = (float **) malloc(sizeof(float *) * 3);
    Ltar = (float **) malloc(sizeof(float *) * 3);
    for(i = 0; i < 3; i++) {
        base[i] = (float *) malloc(sizeof(float) * 3);
        Ltb[i]  = (float *) malloc(sizeof(float) * 3);
        Ltar[i] = (float *) malloc(sizeof(float) * 3);
    }

    numfiles = saclen();

    if(numfiles % 3 != 0) {
        *nerr = 1002;
        error(*nerr, "number of files (%d), must be a multiple of 3", numfiles);
        goto L_8888;
    }
    parse_rotinc(&target,&angle, &verbose, nerr);
    if (*nerr) {
        return;
    }


    j=0;
    while(j+3 <= numfiles) {
        /* Check that all files refer to same time segment and station */
        for(i=0;i<3;i++) {
            s[i] = sacget( j+i, TRUE, nerr );
            if(!nerr) {
                goto L_8888;
            }
        }
        if (statimcmp(s[0],s[1]) || statimcmp(s[0],s[2])) {
            *nerr = 1304;
            error(*nerr, "%d-%d\n"
                  "             Attempt to rotate different components of the same time series\n",j+1,j+3);
            goto L_8888;
        }

        npts = s[0]->h->npts;

        /* construct matrix of base vectors */
        for(i=0;i<3;i++) {
            if(s[i]->h->cmpinc == SAC_FLOAT_UNDEFINED) {
                *nerr = 1336;
                error(*nerr, "CMPINC not set properly in file %s\n", s[i]->m->filename);
                goto L_8888;
            }
            if(s[i]->h->cmpaz == SAC_FLOAT_UNDEFINED) {
                *nerr = 1336;
                error(*nerr, "CMPAZ not set properly in file %s\n", s[i]->m->filename);
                goto L_8888;
            }
            cmpinc = s[i]->h->cmpinc * PI/180.;
            cmpaz  = s[i]->h->cmpaz  * PI/180.;
            base[0][i] = sin(cmpinc) * sin(cmpaz);
            base[1][i] = sin(cmpinc) * cos(cmpaz);
            base[2][i] = cos(cmpinc);

        }
        if((fabs(dot(base[0],base[1],3)) >= EPSILON) ||
           (fabs(dot(base[1],base[2],3)) >= EPSILON) ||
           (fabs(dot(base[0],base[2],3)) >= EPSILON)) {
            error(*nerr = 1002, "Input Coordinate system not Orthogonal\n");
        }

        /* replace matrix of base vectors with its inverse */
        /* construct target rotation matrix and set header variables */
        switch(target) {
        case XYZ: {
            char *comps[3] = {"X", "Y", "Z"};
            float incs[3] = {90., 90., 0.};
            float azs[3]  = {90.,  0., 0.};
            for(i = 0; i < 3; i++) {
                strcpy(s[i]->h->kcmpnm, comps[i]);
                s[i]->h->cmpinc = incs[i];
                s[i]->h->cmpaz = azs[i];
            }
            /* ltar = identity matrix */
            for(i=0;i<3;i++) {
                for(k=0;k<3;k++) {
                    Ltar[i][k] = (i == k) ? 1 : 0;
                }
            }
        }
            break;
        case VNE: {
            char *comps[3] = {"V", "N", "E"};
            float incs[3] = { 0., 90., 90.};
            float azs[3]  = { 0.,  0., 90.};
            for(i = 0; i < 3; i++) {
                strcpy(s[i]->h->kcmpnm, comps[i]);
                s[i]->h->cmpinc = incs[i];
                s[i]->h->cmpaz = azs[i];
            }
            /* ltar = identity matrix */
            for(i=0;i<3;i++) {
                for(k=0;k<3;k++) {
                    Ltar[i][k] = (i+k == 2) ? 1 : 0;
                }
            }
        }
            break;
        case VRT: {
            char *comps[3] = {"V", "R", "T"};
            float incs[3] = { 0., 90., 90.};
            float azs[3]  = { 0.,  0., 90.};
            if(s[0]->h->baz == SAC_FLOAT_UNDEFINED) {
                *nerr = 1336;
                error(*nerr, "baz for files %d-%d\n"
                      "             Cannot rotate to VRT coordinate system",j+1,j+3);
                goto L_8888;
            }
            baz = s[0]->h->baz;
            azs[1] = fmod(180. + baz, 360.);
            azs[2] = fmod(270. + baz, 360.);
            for(i = 0; i < 3; i++) {
                strcpy(s[i]->h->kcmpnm, comps[i]);
                s[i]->h->cmpinc = incs[i];
                s[i]->h->cmpaz = azs[i];
            }
            baz = baz * PI/180.;

            Ltar[0][0]=0;         Ltar[0][1]=0;         Ltar[0][2]=1.0;
            Ltar[1][0]=-sin(baz); Ltar[1][1]=-cos(baz); Ltar[1][2]=0.0;
            Ltar[2][0]=-cos(baz); Ltar[2][1]=sin(baz);  Ltar[2][2]=0.0;
        }
            break;
        case LQT: {
            char *comps[3] = {"L", "Q", "T"};
            float incs[3] = { angle, 90.+angle, 90.};
            float azs[3]  = { 0.,  0., 90.};
            if(s[0]->h->baz == SAC_FLOAT_UNDEFINED) {
                *nerr = 1336;
                error(*nerr, "baz for files %d-%d\n"
                      "             Cannot rotate to LQT coordinate system",j+1,j+3);
                goto L_8888;
            }
            baz = s[0]->h->baz;
            azs[1] = fmod(180. + baz, 360.);
            if (angle != 0.0) {azs[0] = azs[1];}
            azs[2] = fmod(270. + baz, 360.);
            for(i = 0; i < 3; i++) {
                strcpy(s[i]->h->kcmpnm, comps[i]);
                s[i]->h->cmpinc = incs[i];
                s[i]->h->cmpaz = azs[i];
            }

            baz       = baz       * PI/180.;
            angle = angle * PI/180;
            si = sin(angle);
            ci = cos(angle);
            Ltar[0][0]=-sin(baz)*si; Ltar[0][1]=-cos(baz)*si; Ltar[0][2]=ci;
            Ltar[1][0]=-sin(baz)*ci; Ltar[1][1]=-cos(baz)*ci; Ltar[1][2]=-si;
            Ltar[2][0]=-cos(baz);    Ltar[2][1]=sin(baz);     Ltar[2][2]=0.0;
        }
        }
        matrix_mul(Ltar,base,Ltb,3,3,3);

        /* construct trace matrix 3 x npts */
        newtraces = (float **) malloc(sizeof(float *) * 3);
        for(i=0; i<3;i++) {
            traces[i]= s[i]->y;
            newtraces[i] = (float *)malloc(sizeof(float) * npts);
            memset(newtraces[i], 0, sizeof(float) * npts);
        }
        matrix_mul(Ltb,traces,newtraces,3,3,npts);

        /* Copy transformed data and free */
        for(i=0; i<3;i++) {
            memcpy(traces[i], newtraces[i], sizeof(float) * npts);
            free(newtraces[i]);
            sac_extrema(s[i]);
        }

        free(newtraces);

        j += 3;
    }
 L_8888:
    FREE(base);
    FREE(Ltb);
    FREE(Ltar)

    return;
}

#define CHECK_HEADER_INT(a,b,name) do {                                 \
        if(a->h->name != b->h->name) {                                                    \
            printf("Header field %s disagrees (%d vs %d)\n", #name, a->h->name,b->h->name); \
            return 1;                                                   \
        }                                                               \
    } while(0);
#define CHECK_HEADER_FLOAT(a,b,name) do {                                 \
        if(a->h->name != b->h->name) {                                                    \
            printf("Header field %s disagrees (%g vs %g)\n", #name, a->h->name,b->h->name); \
            return 1;                                                   \
        }                                                               \
    } while(0);
#define CHECK_HEADER_STRING(a,b,name) do {                                 \
        if(strcmp(a->h->name,b->h->name) != 0) {                                                    \
            printf("Header field %s disagrees (%s vs %s)\n", #name, a->h->name,b->h->name); \
            return 1;                                                   \
        }                                                               \
    } while(0);

/* check some header files to make sure traces from different components are for the same station and the same time-base */
int statimcmp(sac *a, sac *b) {

    CHECK_HEADER_INT(a,b,npts);
    CHECK_HEADER_INT(a,b,nzyear);
    CHECK_HEADER_INT(a,b,nzjday);
    CHECK_HEADER_INT(a,b,nzhour);
    CHECK_HEADER_INT(a,b,nzmin);
    CHECK_HEADER_INT(a,b,nzsec);
    CHECK_HEADER_INT(a,b,nzmsec);

    CHECK_HEADER_FLOAT(a,b,delta);
    CHECK_HEADER_FLOAT(a,b,b);

    CHECK_HEADER_STRING(a,b,kstnm);

    return 0;
}
void parse_rotinc(enum CoordSystem *target, double *angle, int *verbose, int *nerr) {
    enum LQT_Method imethod;
    int ray_defined;
    double vp,vs,ray,vs2,csi2,appang,tanrotang;
    *nerr = 0;
    ray_defined = FALSE;
    *target     = Unknown_CoordSystem;
    *angle      = 0.0;
    vp  = 5.8;   /* Approximate values valid for crust */
    vs  = 3.36;  /* Original had 3.35*/
    ray = -10.0;

    imethod = Unknown_Method;

    while(lcmore(nerr)) {

        if(lckey("TO$", 4)) {
            /* skip */
        } else if(lckey("VERBOSE$", 5)) {
            *verbose = 1;
            /* Coordinate Systems */
        } else if(lckey("VRT$", 5)) {
            *target = VRT;
        } else if(lckey("GCP$", 5)) {
            *target = VRT;
        } else if(lckey("XYZ$", 5)) {
            *target = XYZ;
        } else if(lckey("VNE$", 5)) {
            *target = VNE;
        } else if(lckey("LQT$", 5)) {
            *target = LQT;
            /* Angle assumes LQT */
        } else if(lkreal("ANGLE$", 10, angle)) {
            *target = LQT;
            imethod = ROTANG;
        } else if(lckey("IP$", 4)) {
            imethod = IP ;
        } else if(lckey("IS$", 4)) {
            imethod = IS;
        } else if(lkreal("VP$", 4, &vp)) {
        } else if(lkreal("VS$", 4, &vs)) {
        } else if(lkreal("RAY$", 5, &ray)) {
            ray_defined = TRUE;
        } else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }

    if(*nerr != 0) {
        return;
    }

    /* Parameter checking */
    switch(*target) {
    case Unknown_CoordSystem:
        *target = LQT;
        /* Fall through to LQT */
    case LQT:
        /* iP, iS or Incidence not set */
        switch(imethod) {
        case Unknown_Method:
            error(*nerr = 1002, "rotation method, please set it "
                  "using iP, iS, or incidence value [Target: %s]", target_str[*target]);
            return;
            break;
        /* Method is IP or IS, but ray parameter is not defined */
        case IP:
        case IS:
            if(!ray_defined) {
                error(*nerr = 1002, "ray parameter, please set it "
                      "using RAY value [Target: %s]", target_str[*target]);
                return;
            }
            break;
        case ROTANG:
            break;
        }
        break;
        /* Rotations around the vertical */
    case XYZ:
    case VNE:
    case VRT:
        switch(imethod) {
        case Unknown_Method:
            break;
        case IP:
        case IS:
        case ROTANG:
            error(*nerr = 1002, "rotinc, incompatible options specificed:\n"
                  "             Vertical Coordinate System with Incidence, iP, or iS");
            return;
        }
        break;
    }


    if(*verbose) {
        printf(" Target Coordinate System: %s\n", target_str[*target]);
    }

    /* Compute Rotation Angle if necessary */
    if(*target == LQT) {
        switch (imethod){
        case Unknown_Method: /* Unreachable */
            break;
        case ROTANG: {
            printf("Input: rotation angle from Vertical  to L\n");
            break;
        }
        case IP:  {
            /* Incident P free-surface response
               (Aki & Richards, 1990) also A&R 2002,
               Problem 5.6, page 184
            */
            printf("Incident P wave, free-surface response\n");
            vs2=vs*vs*ray*ray;
            tanrotang=2*vs*ray*sqrt(1-vs2)/(1-2*vs2);
            *angle=atan(tanrotang)*180./PI;
            appang=*angle;
            if(*verbose) {
                printf("    vP: %.2f km/s vS: %.2f km/s Ray Param: %f s/km\n",vp,vs,ray);
                printf("    Rotation angle: %.2f\n",appang);
            }
            break;
        }
        case IS:  {
            /* Incident SV free-surface response
               (Aki & Richards, 1990) also A&R 2002,
               Problem 5.6, page 184.  Note: A&R have positive V down.
            */
            printf("Incident S wave, free surface response\n");
            if (ray*vp > 1.0) {
                *nerr = 1002;
                error(*nerr, "Incident SV angle: above critical angle");
                return;
            }
            csi2=(1-ray*ray*vp*vp);
            tanrotang=vp*(1-2*ray*ray*vs*vs)/(2*vs*vs*ray*sqrt(csi2));
            /*  The equation for tanrotang only holds for S incidence below the
                critical angle: 1/ray =   vp. tanrotang is the tangent of
                180 - apparent angle for Q so must subtract 90 degrees to get
                TOangle for L*/
            *angle=90.0-atan(tanrotang)*180./PI;
            appang=*angle+90.;
            if(*verbose) {
                printf("    vP: %.2f km/s vS: %.2f km/s Ray Param: %f s/km\n",vp,vs,ray);
                printf("    Rotation angle: %.2f\n",appang);
            }
            break;
        }
        }
    }
}
