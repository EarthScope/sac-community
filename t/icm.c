
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "icm.h"

int verbose = 1;

enum {
    RESP_BASIC = 0,
    RESP_ELMAG,
    RESP_EYEOMG,
    RESP_NORESS_LP,
    RESP_NORESS_IP,
    RESP_NORESS_SP,
    RESP_RS7,
    RESP_RS7_INV,
    RESP_RSK,
    RESP_RSK_INV,
    RESP_SANDIA,
    RESP_SRO,
    RESP_RSTN,
    RESP_REFTEK,
    RESP_GENERAL,
    RESP_PORTABLE,
    RESP_LLL,
};
#define RESP_BASIC_PADDING 0, 0, 0, 0,0,0,""

typedef void (*response)(int nfreq, double dfreq, double xre[], double xim[]);

struct resp {
    int type;
    response func;
    char *file;
    /* elmag */
    double freeper, mag;
    /* eyeomg */
    int nzeros;
    /* reftek */
    double damping;
    double crfrq, hpfrq;
    char name[8];
    
};


void
error() {
    exit(-1);
}


int
dcomp(double *x, double *xt, int n) {
    int i;
    int flag = 1;
    double v;
    for(i = 0; i < n; i++) {
        v = 1e-8  + 1e-5 * fabs(xt[i]);
        if(fabs(x[i] - xt[i]) > v) { /* Taken from numpy.isclose() May,2015 */
            if(verbose) {
                fprintf(stderr, "Error: %d/%d val: %.18e comp: %.18e tol: %.18e\n", i,n,xt[i],x[i], v);
            }
            flag = 0;
        }
    }
    return flag;
}


void
response_read(struct resp *R, int *nfreq, double *dfreq, double **xrep, double **ximp) {
    FILE *fp;
    int i;
    double *xre;
    double *xim;

    if((fp = fopen(R->file, "r")) == NULL) {
        fprintf(stderr, "icm: error opening file: %s\n", R->file);
        error();
    }
    
    if(R->type == RESP_ELMAG) {
        if(fscanf(fp, "freeperiod %le magnification %le\n", &R->freeper, &R->mag) != 2) {
            fprintf(stdout, "Error reading response header [elmag]\n");
            error();
        }
    }
    if(R->type == RESP_EYEOMG) {
        if(fscanf(fp, "nzeros %d\n", &R->nzeros) != 1) {
            fprintf(stdout, "Error reading response header [eyeomg]\n");
            error();
        }
    }
    
    if(fscanf(fp, "npts %d dfreq %le\n", nfreq, dfreq) != 2) {
        fprintf(stdout, "Error reading response header\n");
        error();
    }

    xre  = malloc(sizeof(double) * *nfreq);
    xim  = malloc(sizeof(double) * *nfreq);
    for(i = 0; i < *nfreq; i++) {
        if(fscanf(fp, "%le %le\n", &xre[i], &xim[i]) != 2) {
            fprintf(stdout, "Error reading response at point %d\n", i);
            error();
        }
    }
    fclose(fp);
    *xrep = xre;
    *ximp = xim;
}

void
response_write(struct resp R, int nfreq, double dfreq, double *xre, double *xim) {
    FILE *fp;
    int i;
    if((fp = fopen(R.file, "w")) == NULL) {
        error();
    }
    if(R.type == RESP_ELMAG) {
        fprintf(fp, "freeperiod %le magnification %le\n", R.freeper, R.mag);
    }
    if(R.type == RESP_EYEOMG) {
        fprintf(fp, "nzeros %d\n", R.nzeros);
    }

    if(fprintf(fp, "npts %d dfreq %.18le\n", nfreq, dfreq) < 0) {
        fprintf(stdout, "Error writing response header\n");
        error();
    }

    for(i = 0; i < nfreq; i++) {
        if(fprintf(fp, "%.18le %.18le\n", xre[i], xim[i]) < 0) {
            fprintf(stdout, "Error writing response at point %d\n", i);
            error();
        }
    }
    fclose(fp);
}

void
response_compute_internal(int nfreq, double dfreq, struct resp R, double **xrep, double **ximp) {
    int nerr;
    double *xre, *xim;

    xre = malloc(sizeof(double) * nfreq);
    xim = malloc(sizeof(double) * nfreq);

    switch(R.type) { 
    case RESP_BASIC:  R.func(nfreq, dfreq, xre, xim);break;
    case RESP_ELMAG:  elmag(nfreq, dfreq, xre, xim, R.freeper, R.mag, &nerr); break;
    case RESP_EYEOMG: eyeomg(nfreq, dfreq, xre, xim, R.nzeros); break;
    case RESP_NORESS_LP: noress(nfreq, dfreq, xre, xim, "LP      ", 8); break;
    case RESP_NORESS_IP: noress(nfreq, dfreq, xre, xim, "IP      ", 8); break;
    case RESP_NORESS_SP: noress(nfreq, dfreq, xre, xim, "SP      ", 8); break;
    case RESP_RS7:     rs7(nfreq, dfreq, xre, xim, "BLAH   "); break;
    case RESP_RS7_INV: rs7(nfreq, dfreq, xre, xim, "ON7S.E  "); break;
    case RESP_RSK:     rsk(nfreq, dfreq, xre, xim, "BLAH   "); break;
    case RESP_RSK_INV: rsk(nfreq, dfreq, xre, xim, "ON7S.N  "); break;
    case RESP_SANDIA:  sandia(nfreq, dfreq, xre, xim, R.name, 4, &nerr); break;
    case RESP_SRO:     sro(nfreq, dfreq, xre, xim, R.name, 4); break;
    case RESP_RSTN:    rstn(nfreq, dfreq, xre, xim, R.name, 6, &nerr); break;
    case RESP_REFTEK:  reftek(nfreq, dfreq, xre, xim, R.freeper, R.damping, R.crfrq, R.hpfrq); break;
    case RESP_GENERAL: general(nfreq, dfreq, xre, xim, R.nzeros, R.freeper, R.damping, R.mag); break;
    case RESP_PORTABLE: portable(nfreq, dfreq, xre, xim, R.freeper, R.damping, R.crfrq); break;
    case RESP_LLL: lll(nfreq, dfreq, xre, xim, R.name, 8, R.freeper, R.damping, &nerr); break;
    }

    *xrep = xre;
    *ximp = xim;
}


void
response_compute(int nfreq, double dfreq, struct resp R) {
    double *xre, *xim;

    response_compute_internal(nfreq, dfreq, R, &xre, &xim);

    response_write(R, nfreq, dfreq, xre, xim);

    free(xre);
    free(xim);
}

int
response_compare(struct resp R) {

    int nfreq;
    double df;
    double *xre, *xim;
    double *xret, *ximt;

    response_read(&R, &nfreq, &df, &xret, &ximt);

    response_compute_internal(nfreq, df, R, &xre, &xim);

    if(!dcomp(xre, xret, nfreq)) {
        return 0;
    }
    if(!dcomp(xim, ximt, nfreq)) {
        return 0;
    }

    free(xre);
    free(xim);
    free(xret);
    free(ximt);

    return 1;
}


struct resp Resps[] = {
    { RESP_BASIC, acc, "acc.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, bbdisp, "bbdisp.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, bbvel, "bbvel.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, benbog, "benbog.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, clh, "clh.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, clz, "clz.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, cmh, "cmh.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, cmz, "cmz.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, csh, "csh.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, csz, "csz.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, dss, "dss.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, dwwssn, "dwwssn.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, ekalp6, "ekalp6.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, ekasp2, "ekasp2.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, gbalp, "gbalp.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, gbasp, "gbasp.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, gsref, "gsref.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, hfslpwb, "hfslpwb.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, hs3, "hs3.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, llsn, "llsn.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, lrsmlp, "lrsmlp.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, lrsmsp, "lrsmsp.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, noresshf, "noresshf.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, oldbb, "oldbb.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, oldkir, "oldkir.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, ptbllp, "ptbllp.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, redkir, "redkir.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, rsl, "rsl.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, rsm, "rsm.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, snla3, "snla3.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, vel, "vel.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, wa, "wa.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, wabn, "wabn.txt", RESP_BASIC_PADDING }, 
    { RESP_BASIC, wiech, "wiech.txt", RESP_BASIC_PADDING }, 
    { RESP_BASIC, wwlpbn, "wwlpbn.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, wwsp, "wwsp.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, wwspbn, "wwspbn.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, ykalp, "ykalp.txt", RESP_BASIC_PADDING },
    { RESP_BASIC, ykasp, "ykasp.txt", RESP_BASIC_PADDING },

    { RESP_ELMAG, NULL, "elmag.txt", 15, 375, 0, 0,0,0,"" },
    { RESP_EYEOMG, NULL, "eyeomg.txt", 0,0, 5, 0,0,0,""},
    { RESP_NORESS_LP, NULL, "noress_lp.txt", RESP_BASIC_PADDING },
    { RESP_NORESS_IP, NULL, "noress_ip.txt", RESP_BASIC_PADDING },
    { RESP_NORESS_SP, NULL, "noress_sp.txt", RESP_BASIC_PADDING },
    { RESP_RS7, NULL, "rs7.txt", RESP_BASIC_PADDING },
    { RESP_RS7_INV, NULL, "rs7_inv.txt", RESP_BASIC_PADDING },
    { RESP_RSK, NULL, "rsk.txt", RESP_BASIC_PADDING },
    { RESP_RSK_INV, NULL, "rsk_inv.txt", RESP_BASIC_PADDING },
    { RESP_SANDIA, NULL, "sandia_ol.txt", 0,0,0, 0,0,0,"OL" },
    { RESP_REFTEK, NULL, "reftek.txt", 20,0,0, 0.7, 1, -0.5, ""},
    { RESP_GENERAL, NULL, "general.txt", 20.0,1e3,3, 0.7,0,0, ""},
    { RESP_PORTABLE, NULL, "portable.txt", 20.0,1e3,0, 0.7,1.0,0, ""},
    { RESP_LLL, NULL, "lll.txt", 20.0,0,0, 0.7,0,0, "BB" },
    { RESP_RSTN, NULL, "sro_rstn.txt", 0,0,0, 0,0,0,"CPKL.Z" },
    { RESP_SRO, NULL, "sro_bb.txt", 0,0,0, 0,0,0,"BB" },
};

int
main(int argc, char *argv[]) {
    int i, n;
    n = sizeof(Resps)/sizeof(struct resp);
    if(argc > 1 && strcmp(argv[1], "-v") == 0) {
        verbose = 1;
    }
    for( i = 0;  i < n; i++) {
        //printf("%d/%d: %s\n", i+1,n,Resps[i].file);
        //response_compute(513, 1e-2, Resps[i]);
        if(! response_compare(Resps[i]) ) {
            printf("%3d/%3d: %s - Error\n", i+1,n,Resps[i].file);
            if(verbose) {
                error();
            }
        } else {
            if(verbose) {
                printf("%3d/%3d: %s - Ok\n", i+1,n,Resps[i].file);
            }
        }
    }
    return 0;
    
}
