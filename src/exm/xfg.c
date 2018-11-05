/** 
 * @file   xfg.c
 * 
 * @brief  Function Generate
 * 
 */

#include <string.h>
#include <math.h>

#include "exm.h"
#include "msg.h"
#include "cpf.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "co.h"
#include "bool.h"
#include "dfm.h"

#include "bot.h"
#include "ucf.h"
#include "ssi.h"
#include "clf.h"
#include "dbh.h"
#include "dff.h"

#include "debug.h"

#include <fstr.h>

DFM_EXTERN
EXM_EXTERN

#define PI M_PI

/** 
 * Generate a Function and store it in memory
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   961031:  ninf and nhst were changed to norid and nevid for
 *             compatability with the CSS format.  Also, the variables
 *             *b and *e were replaced with *begin and *ennd (See hdr.h)
 *             maf
 * @date   911119:  Added data-set storage stuff.
 * @date   910516:  Fixed funcgen problem reported by George Helffrich of
 *             Carnegie Institute of Washington. Funcgen was writing
 *             one too many points in the data block (wct).
 * @date   890907:  Added unit impulse function.
 * @date   870923:  Deleted ".saf" suffix from seismogram name.
 * @date   870721:  Fixed bug involving an incorrect error message in RANDOM.
 * @date   860327:  Fixed several small bugs in SEISMOGRAM option.
 * @date   860218:  Fixed bug in TRIANGLE option.
 * @date   850801:  Changes in argument list for RDSAC.
 * @date   840120:  Added random noise generator function.
 * @date   821123:  Added a new function containing a sample data file.
 * @date   820805:  Major restructuring and addition of new functions.
 * @date   820316:  Modified sine wave generator to include frequency.
 * @date   810528:  Added LINE.
 * @date   810414:  Minor changes relating to common block reorganization.
 * @date   810120:  Changed to error message retrieval from disk.
 * @date   801208:  Changed file generation to use CRSAC.
 * @date   800728:  Added IMPULSE.
 *
 */
void
xfg(int *nerr) {
    char kfile[MCPFN + 1];
    int iseed, j, j1, jdfl = 0, ndx1, ndx2, ndxh, nlen, n;
    static int nra;
    int itmp;
    double dtmp[2];
    double arg, con, del;
    double arg0;
    sac *s;
    *nerr = 0;
    memset(kfile, 0, sizeof(kfile));
    ndx1 = ndx2 = 0;
    s = NULL;
    while (lcmore(nerr)) {

        /* -- select one of the possible functions. */
        if (lclist((char *) kmexm.kfgtp, 9, cmexm.nfgtp, &cmexm.ifgtp)) {
            if (cmexm.ifgtp == 5) {
                if (lcra(0, 2, cmexm.fgsico, &nra)) {   /* do nothing */
                }
            } else if (cmexm.ifgtp == 6) {
                if (lcra(0, 2, cmexm.fglico, &nra)) {   /* do nothing */
                }
            } else if (cmexm.ifgtp == 7) {
                if (lcra(0, 3, cmexm.fgquco, &nra)) {   /* do nothing */
                }
            } else if (cmexm.ifgtp == 8) {
                if (lcra(0, 4, cmexm.fgcuco, &nra)) {   /* do nothing */
                }
            } else if (cmexm.ifgtp == 9) {
                if (lcra(0, 2, dtmp, &nra)) {
                    if(nra > 0 && dtmp[0] <= 0) {
                        error(*nerr = 1002, "'Number of Files', must be positive");
                        break;
                    }
                    for(j = 0; j < nra; j++) {
                        cmexm.fgraco[j] = dtmp[j];
                    }
                }
            } else if (cmexm.ifgtp == 12) {
                if (lcra(0, 1000, cmexm.fgistr, &nra)) {        /* do nothing */
                }
            }

        }

        /* -- "DELTA v":  change sampling interval. */
        else if (lkreal("D#ELTA$", 8, &cmexm.fgdel)) {       /* do nothing */
        }

        /* -- "BEGIN v":  change beginning value. */
        else if (lkreal("BE#GIN$", 8, &cmexm.fgbeg)) {      /* do nothing */
        }

        /* -- "NPTS n":  change number of data points in function. */
        else if (lkint("N#PTS$", 7, &itmp)) {
            if(itmp <= 0) {
                error(*nerr = 1002, "NPTS, must be positive");
                break;
            }
            cmexm.nfgpts = itmp;
        }

        /* -- Bad syntax. */
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }

    /* - The above loop is over when one of two conditions has been met:
     *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
     *   (2) All the tokens in the command have been successfully parsed. */
    DEBUG("\n");
    if (*nerr != 0)
        return;

    /* EXECUTION PHASE: */

    /* - Initialize memory manager and data file list. */
    sacclear();

    /* - Create room in memory for generated function.
     *   All data currently in memory is destroyed. */

    /* -- Random number generator function may generate more than one file.
     *    (Force number of data points to be even in this case.) */
    if (cmexm.ifgtp == 9) {
        cmexm.nfgpts = 2 * ((cmexm.nfgpts - 1) / 2 + 1);
        n = (int) (Fgraco[1] + 0.1);
        for (jdfl = 1; jdfl <= n; jdfl++) {

            /* --- save some related information */
            if (jdfl <= 9) {
                sprintf(kfile, "%s%1d", "RANDOM0", jdfl);
            } else {
                sprintf(kfile, "%s%2d", "RANDOM", jdfl);
            }

            s = sac_new();
            s->m->filename = fstrdup(kfile, MCPFN + 1);
            s->h->npts = cmexm.nfgpts;
            sac_alloc(s);
            sacput(s);
        }                       /* end for */

        /* -- All other functions generate one file only.
         * -- Sample seismogram is unique in length. */
    } /* end if ( cmexm.ifgtp == 9 ) */
    else {
        DEBUG("\n");
        jdfl = 1;

        s = sac_new();

        s->m->filename = fstrdup(kmexm.kfgtp[cmexm.ifgtp - 1], 9);
        s->h->npts = cmexm.nfgpts;
        sac_alloc(s);
        sacput(s);

    }

    /* - Set up new header. */
    s->h->b = cmexm.fgbeg;
    s->h->delta = cmexm.fgdel;
    s->h->npts = cmexm.nfgpts;
    sac_be(s);

    fstrncpy(s->h->kevnm, 17, "FUNCGEN: ", 9);
    fstrncpy(s->h->kevnm + 9, 17 - 10, kmexm.kfgtp[cmexm.ifgtp - 1],
             strlen(kmexm.kfgtp[cmexm.ifgtp - 1]));

    /*     ndx2=ndx1+nfgpts
     * - Write one less point to data block (modificatoin 5/16/91) */
/*	ndx2 = ndx1 + cmexm.nfgpts - 1; */
    ndx2 = cmexm.nfgpts - 1;

    /* - Generate function. */
    switch (cmexm.ifgtp) {
        case 1:
            goto L_2010;
        case 2:
            goto L_2020;
        case 3:
            goto L_2030;
        case 4:
            goto L_2040;
        case 5:
            goto L_2050;
        case 6:
            goto L_2060;
        case 7:
            goto L_2070;
        case 8:
            goto L_2080;
        case 9:
            goto L_2090;
        case 10:
            goto L_2100;
        case 11:
            goto L_2110;
        case 12:
            goto L_2120;
    }

    /* -- Impulse. */

  L_2010:

    for (j = 0; j <= ndx2; j++) {
        s->y[j] = 0;
    }
    s->y[ndx2 / 2] = 1.0;
    DEBUG("\n");
    goto L_8888;

    /* -- Step function. */

  L_2020:
    j1 = s->h->npts / 2;

    for (j = 0; j <= j1; j++) {
        s->y[j] = 0;
    }

    for (j = j1 + 1; j <= ndx2; j++) {
        s->y[j] = 1.0;
    }
    goto L_8888;

    /* -- Boxcar. */

  L_2030:
    j1 = s->h->npts / 3;

    for (j = 0; j <= j1; j++) {
        s->y[j] = 0;
    }
    for (j = j1 + 1; j <= (2 * j1); j++) {
        s->y[j] = 1;
    }
    for (j = 2 * j1 + 1; j <= ndx2; j++) {
        s->y[j] = 0;
    }
    goto L_8888;

    /* -- Triangle function. */

  L_2040:
    j1 = s->h->npts / 4;

    for (j = 0; j <= j1; j++) {
        s->y[j] = 0;
    }
    con = 1. / (float) (j1);
    for (j = j1 + 1; j <= (2 * j1); j++) {
        s->y[j] = (j - j1) * con;
    }
    for (j = 2 * j1 + 1; j <= (3 * j1); j++) {
        s->y[j] = 1.0 - (j - 2 * j1) * con;
    }
    for (j = 3 * j1 + 1; j <= ndx2; j++) {
        s->y[j] = 0.0;
    }
    goto L_8888;

    /* -- Sine wave. */

  L_2050:
    del = 2. * PI * Fgsico[1] * s->h->delta;
    arg0 = 2. * PI * (Fgsico[1] * s->h->b + Fgsico[2] / 360.);
    for (j = 0; j <= ndx2; j++) {
        arg = arg0 + j * del;
        s->y[j] = sin(arg);
    }
    goto L_8888;

    /* -- Linear equation (Coefficients are linear multiplier and constant [ax+b]). */

  L_2060:
    for (j = 0; j <= ndx2; j++) {
        arg = s->h->b + j * s->h->delta;
        s->y[j] = Fglico[1] * arg + Fglico[2];
    }
    goto L_8888;

    /* -- Quadratic equation. */

  L_2070:
    for (j = 0; j <= ndx2; j++) {
        arg = s->h->b + j * s->h->delta;
        s->y[j] = Fgquco[1] * powi(arg, 2) + Fgquco[2] * arg + Fgquco[3];
    }
    goto L_8888;

    /* -- Cubic equation. */

  L_2080:
    for (j = 0; j <= ndx2; j++) {
        arg = s->h->b + j * s->h->delta;
        s->y[j] =
            Fgcuco[1] * powi(arg, 3) + Fgcuco[2] * powi(arg,
                                                        2) + Fgcuco[3] * arg +
            Fgcuco[4];
    }
    goto L_8888;

    /* -- Random number generator.
     *    (Call to putfil before getfil is to set up each header.) */

  L_2090:
    iseed = (int) (Fgraco[2] + 0.1);
    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        s->h->user0 = (float) (iseed);
        for (j = 0; j <= (s->h->npts - 1); j += 2) {
            gauss(&iseed, &s->y[j], &s->y[j + 1]);
        }
        extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax,
               &s->h->depmen);
    }
    goto L_8888;

    /* -- Sample data file. */

  L_2100:
    zbasename(kfile, MCPFN + 1);
    crname(kfile, MCPFN + 1, KDIRDL, "seismogram", 11, nerr);

    if (*nerr != 0)
        goto L_8888;

    rdsac(jdfl, kfile, MCPFN + 1, FALSE, TRUE, &nlen, &ndxh, &ndx1, &ndx2,
          nerr);
    if (*nerr != 0)
        goto L_8888;

    goto L_8888;

    /* -- Unit impulse (contains a 1 as the first data point. */

  L_2110:

    s->y[0] = 1.0;
    for (j = 1; j <= ndx2; j++) {
        s->y[j] = 0.0;
    }
    goto L_8888;

    /* -- impulse string  */

  L_2120:

    for (j = 0; j <= ndx2; j++) {
        s->y[j] = 0;
    }

    /* subtracted 1 from fgistr to convert 
       FORTRAN style input arguments to C compatible 
       PG 1/4/01. */
    for (j = 0; j <= nra - 1; j++) {
        j1 = (int) (cmexm.fgistr[j] - 1 + RNDOFF);
        if (j1 < s->h->npts && j1 >= 0) {
            s->y[j1] = 1.0;
        }
    }

    goto L_8888;

  L_8888:
    if (cmexm.ifgtp != 9 && cmexm.ifgtp != 10 && *nerr == 0) {
        extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax,
               &s->h->depmen);
    }

    if (*nerr == 0) {           /* if no error occured */
        cmdfm.nfilesFirst = 0;
        cmdfm.nreadflag = LOW;
        cmdfm.lread = TRUE;
        sacToSeisMgr(TRUE, FALSE, TRUE, nerr);
        cmdfm.lread = FALSE;
    }
    setrng();
    DEBUG("\n");
    return;

}                               /* end of function */
