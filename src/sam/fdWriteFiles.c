
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sam.h"
#include "hdr.h"
#include "amf.h"
#include "dfm.h"
#include "bool.h"
#include "ucf.h"
#include "ncpf.h"

#include "co.h"
#include "msg.h"
#include "dff.h"

static void aphdr(int newnpts, sac * s);
static void gdhdr(int newnpts, sac * s);
static void irhdr(int newnpts, sac * s);
static void fillNZ(sac * s);

void                            /* FUNCTION */
fdWriteFiles(float *memptr[10], char *kprefix, float *userData, int newnpts,
             int *nerr) {

    /* index sacmem for amplitude, phase, group delay,
       and the impulse response. */
    int xbegin = 0, idx, jdx, nlcmem;

    char kname[MCPFN], ksuffix[3][6];
    sac *s;
    float *ptr, amph[2][2 * NDATPTS - 2];

    *nerr = 0;

    /* handle strings */
    if (strlen(kprefix) > MCPFN - 4)
        kprefix[MCPFN - 4] = '\0';
    strcpy(ksuffix[0], ".spec");
    strcpy(ksuffix[1], ".gd");
    strcpy(ksuffix[2], ".imp");

    /* Determine the begin of the impulse */
    for (ptr = memptr[9]; *ptr == 0.0; ptr++)
        xbegin++;

    /* fill the amplitude and phase array */
    for (idx = 0; idx < NDATPTS; idx++) {
        amph[0][idx] = memptr[6][idx];
        amph[1][idx] = memptr[7][idx];
    }

    for (; idx < 2 * NDATPTS - 2; idx++) {
        amph[0][idx] = memptr[6][2 * NDATPTS - idx - 2];
        amph[1][idx] = -memptr[7][2 * NDATPTS - idx - 2];
    }

    s = sac_new();

    /* fill some fields. */
    for (idx = 0; idx < 9; idx++)       /* user fields */
        *(&(s->h->user0) + idx) = userData[idx];

    switch ((int) (s->h->user0 + 0.5)) {
        case 1:
            strcpy(s->h->kuser0, "lowpass ");
            break;
        case 2:
            strcpy(s->h->kuser0, "highpass");
            break;
        case 3:
            strcpy(s->h->kuser0, "bandpass");
            break;
        case 4:
            strcpy(s->h->kuser0, "bandrej ");
            break;
        default:
            strcpy(s->h->kuser0, "-12345  ");
            break;
    }

    switch ((int) (s->h->user1 + 0.5)) {
        case 1:
            strcpy(s->h->kuser1, "Butter  ");
            break;
        case 2:
            strcpy(s->h->kuser1, "Bessel  ");
            break;
        case 3:
            strcpy(s->h->kuser1, "C1      ");
            break;
        case 4:
            strcpy(s->h->kuser1, "C2      ");
            break;
        default:
            strcpy(s->h->kuser1, "-12345  ");
            break;
    }

    fillNZ(s);                  /* time fields */

    sac_set_float(s, SAC_B, 0.0);
    sac_set_float(s, SAC_SB, 0.0);
    s->h->nvhdr = 6;
    s->h->idep = IUNKN;
    s->h->iztype = IB;
    s->h->leven = TRUE;
    s->h->lpspol = TRUE;
    s->h->lovrok = TRUE;
    s->h->lcalda = FALSE;

    for (jdx = 0; jdx < 3; jdx++) {     /* loop between output files. */

        /* fill other header fields specific to the data */
        switch (jdx) {
            case 0:
                aphdr(newnpts, s);
                nlcmem = 6;
                break;
            case 1:
                gdhdr(newnpts, s);
                nlcmem = 8;
                break;
            case 2:
                irhdr(newnpts, s);
                nlcmem = 9;
                break;
            default:
                goto L_ERROR;
        }
        /* Get file name */
        sprintf(kname, "%s%s", kprefix, ksuffix[jdx]);

        /* Write data to disk */
        switch (jdx) {
            case 0:
                /* nptwr = 2 * NDATPTS - 2; */
                //sac_data_write2(fileDescriptor, amph[0], amph[1], s->h->npts,
                //                FALSE, nerr);
                s->y = amph[0];
                s->x = amph[1];
                break;
            case 1:
                //sac_data_write1(fileDescriptor, memptr[nlcmem], s->h->npts,
                //                FALSE, nerr);
                s->y = memptr[nlcmem];
                s->x = NULL;
                break;
            case 2:
                //sac_data_write1(fileDescriptor, memptr[nlcmem] + xbegin,
                //                s->h->npts, FALSE, nerr);
                s->y = memptr[nlcmem] + xbegin;
                s->x = NULL;
                break;
        }
        sac_write_r(s, kname, TRUE, FALSE, nerr);
        s->y = NULL;
        s->x = NULL;
        /* Close file */
        //zclose(&fileDescriptor, nerr);
        //fileDescriptor = 0;
    }                           /* end for */

  L_ERROR:
    if (*nerr) {
        setmsg("ERROR", *nerr);
        outmsg();
        clrmsg();
    }

    sac_free(s);

    if (saclen() > 0) {
        if (!(s = sacget(0, TRUE, nerr))) {

        }
    }
    //getfil ( 1 , TRUE , &unused1 , &unused2 , &unused3 , nerr ) ;

    //if (fileDescriptor)
    //    zclose(&fileDescriptor, nerr);
}

static void
aphdr(int newnpts, sac * s) {
    s->h->nsnpts = newnpts;
    s->h->npts = 2 * NDATPTS - 2;
    sac_set_float(s, SAC_SDELTA, s->h->user6);
    sac_set_float(s, SAC_DELTA, 1. / (SDT(s) * (double) (s->h->npts)));
    s->h->iftype = IAMPH;

    strcpy(s->h->kevnm, "FD: AMP/PH");
}

static void
gdhdr(int newnpts, sac * s) {
    s->h->nsnpts = newnpts;
    s->h->npts = NDATPTS;
    sac_set_float(s, SAC_SDELTA, s->h->user6);
    sac_set_float(s, SAC_DELTA, 1. / (SDT(s) * (double) (s->h->npts)));
    s->h->iftype = ITIME;

    strcpy(s->h->kevnm, "FD: GROUP DELAY");
}

static void
irhdr(int newnpts, sac * s) {
    s->h->nsnpts = NDATPTS;
    s->h->npts = newnpts;
    sac_set_float(s, SAC_DELTA, s->h->user6);
    sac_set_float(s, SAC_SDELTA, 1. / (DT(s) * (double) (s->h->npts)));
    s->h->iftype = ITIME;

    strcpy(s->h->kevnm, "FD: IMPULSE");
}

static void
fillNZ(sac * s) {
    double time = tmGetEpochTime();
    int year, month, day, hour, minute;
    float second;

    tmDecodeEpochTime(time, &year, &month, &day, &hour, &minute, &second);

    s->h->nzyear = year;
    s->h->nzjday = MDtoDoy(month, day, isLeapYear(year));
    s->h->nzhour = hour;
    s->h->nzmin = minute;
    s->h->nzsec = (int) second;
    s->h->nzmsec = (second - s->h->nzsec) * 1000;
}
