
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sam.h"
#include "hdr.h"
#include "amf.h"
#include "bool.h"
#include "dfm.h"
#include "ucf.h"

#include "co.h"
#include "msg.h"
#include "dff.h"
#include "defs.h"

void aphdrw(int newnpts, int nFreq, sac * s);
void gdhdrw(int newnpts, int nFreq, sac * s);
void irhdrw(int newnpts, int nFreq, sac * s);

void                            /* FUNCTION */
fdWhitenWrite(float *resp[4], char *kprefix, float *userData, int newnpts,
              int nFreq, int *nerr) {
    /* index sacmem for Amplitude, Phase, 
       and the impulse response. */

    int fileDescriptor = 0, idx, jdx, nlcmem;
    char kname[MCPFN], ksuffix[3][6];
    sac *s;
    float *bufout = NULL, **amph = NULL;

    *nerr = 0;

    /* allocate room for spectral data */
    amph = (float **) calloc(2, sizeof(float *));
    if (amph == NULL) {
        *nerr = 301;
        goto L_ERROR;
    }

    amph[0] = (float *) malloc((nFreq) * sizeof(float));
    amph[1] = (float *) malloc((nFreq) * sizeof(float));

    if (amph[0] == NULL || amph[1] == NULL) {
        *nerr = 301;
        goto L_ERROR;
    }

    /* handle strings */
    if (strlen(kprefix) > MCPFN - 4)
        kprefix[MCPFN - 4] = '\0';
    strcpy(ksuffix[0], ".spec");
    strcpy(ksuffix[1], ".imp");
    strcpy(ksuffix[2], ".gd");

    s = sac_new();
    /* fill the amplitude and phase array */
    for (idx = 0; idx < nFreq; idx++) {
        amph[0][idx] = resp[1][idx];
        amph[1][idx] = resp[2][idx];
    }

    /* fill some fields. */
    for (idx = 0; idx < 9; idx++)       /* user fields */
        *((&s->h->user0) + idx) = userData[idx];

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
        case 5:
            strcpy(s->h->kuser0, "whiten  ");
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
        case 5:
            strcpy(s->h->kuser1, "whiten  ");
            break;
        default:
            strcpy(s->h->kuser1, "-12345  ");
            break;
    }

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
                aphdrw(newnpts, nFreq, s);
                nlcmem = 1;
                break;
            case 1:
                irhdrw(newnpts, nFreq, s);
                nlcmem = 0;
                break;
            case 2:
                gdhdrw(newnpts, nFreq, s);
                nlcmem = 3;
                break;
            default:
                goto L_ERROR;
        }

        /* Get file name */
        sprintf(kname, "%s%s", kprefix, ksuffix[jdx]);


        /* Write data to disk */
        switch (jdx) {
            case 0:
                s->y = amph[0];
                s->x = amph[1];
                break;
            case 1:
            case 2:
                s->y = resp[nlcmem];
                break;
        }

        sac_write_r(s, kname, TRUE, FALSE, nerr);
        s->y = NULL;
        s->x = NULL;
    }

  L_ERROR:

    if (*nerr) {
        setmsg("ERROR", *nerr);
        outmsg();
        clrmsg();
    }

    if (saclen() > 0) {
        if (!(s = sacget(0, TRUE, nerr))) {
        }
    }
    //getfil ( 1 , TRUE , &unused1 , &unused2 , &unused3 , nerr ) ;

    if (amph) {
        FREE(amph[0]);
        FREE(amph[1]);
        FREE(amph);
    }
    FREE(bufout);
    if (fileDescriptor)
        zclose(&fileDescriptor, nerr);
}

void
aphdrw(int newnpts, int nFreq, sac * s) {
    s->h->nsnpts = newnpts;
    s->h->npts = nFreq;
    sac_set_float(s, SAC_SDELTA, s->h->user6);
    sac_set_float(s, SAC_DELTA, 1. / (SB(s) * (double) (nFreq)));
    s->h->iftype = IAMPH;

    strcpy(s->h->kevnm, "FD: AMP/PH");
}

void
gdhdrw(int newnpts, int nFreq, sac * s) {
    s->h->nsnpts = newnpts;
    s->h->npts = nFreq / 2;
    sac_set_float(s, SAC_SDELTA, s->h->user6);
    sac_set_float(s, SAC_DELTA, 1. / (SB(s) * nFreq));
    s->h->iftype = ITIME;

    strcpy(s->h->kevnm, "FD: GROUP DELAY");
}

void
irhdrw(int newnpts, int nFreq, sac * s) {
    s->h->nsnpts = nFreq;
    s->h->npts = newnpts;
    sac_set_float(s, SAC_DELTA, s->h->user6);
    sac_set_float(s, SAC_SDELTA, 1. / (DT(s) * (double) (s->h->npts)));
    s->h->iftype = ITIME;

    strcpy(s->h->kevnm, "FD: IMPULSE");
}
