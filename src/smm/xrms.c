
#include <stdio.h>
#include <math.h>

#include "smm.h"
#include "lhf.h"
#include "dfm.h"
#include "amf.h"
#include "bool.h"
#include "msg.h"
#include "errors.h"

#include "cpf.h"
#include "dff.h"

SMM_EXTERN
LHF_EXTERN

void /*FUNCTION*/
xrms(nerr)
     int *nerr;
{
    int ifpick, j, jdfl, nlnnoise, nlnsignal, nofnoise, nofsignal;
    float rms, sumsq, sumsqnoise, sumsqsignal;
    double tmax, tmin;
    sac *s;
        /*=====================================================================
	 * PURPOSE: To parse and execute the action command RMS.
	 *          This command measures the root mean square value
	 *          within a measurement time window.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  smm/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    dfm:     ndfl, kdfl
	 *    smm:     lmtw, kmtw, omtw
	 *    lhf:     kfhdr
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    hdr:
	 *    smm:     lnoisemtw, knoisemtw, onoisemtw, irmspick
	 *    mem:     sacmem
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, cfmt, cresp, lkrtw, lklist, vflist, vfeven,
	 *             getfil, getatw
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920320:  Added square root per Hauk, Walter, et.al.
	 *    911021:  Removed  1/2 log function, to do a sqrt per Teri Hauk
	 *             and Bill Walter.
	 *    890223:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  890223
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    /* PARSING PHASE: */

    /* - Loop on each token in command: */

    while (lcmore(nerr)) {
        /* -- "NOISE ON|OFF|rtw:  set noise window option. */
        if (lkrtw("NOISE$", 7, &cmsmm.lnoisemtw, (char *) kmsmm.knoisemtw, 9, cmsmm.onoisemtw)) {       /* do nothing */
        }

        /* -- "TO hdrvar":  the name of the header variable to store measurement. */
        else if (lklist("TO$", 4, (char *) kmlhf.kfhdr[40], 9, 10, &cmsmm.irmspick)) {  /* do nothing */
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

    if (*nerr != 0)
        goto L_8888;

    /* CHECKING PHASE: */

    /* - Test for a non-null data file list. */

    vflist(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Make sure each file is an evenly spaced time series file. */

    vfeven(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* EXECUTION PHASE: */

    /* - Determine offset in header arrays for requested time pick. */

    ifpick = 40 + cmsmm.irmspick;

    /* - Perform the requested function on each file in DFL. */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        /* -- Get next file from the memory manager.
         *    (Header is moved into common blocks CMHDR and KMHDR.) */
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        //getfil( jdfl, TRUE, &nlenfile, &ndxfile, &notused, nerr );

        /* -- Determine signal measurement window. */
        if (cmsmm.lmtw) {
            getatw((char *) kmsmm.kmtw, 9, cmsmm.omtw, &tmin, &tmax, &nofsignal,
                   &nlnsignal, nerr);
            if (*nerr != 0) {
                goto L_8888;
            }
            if(tmin < B(s)) {
                warning(8012, "(%.5g < %.5g)", tmin, B(s));
                outmsg();
                clrmsg();
                tmin = fmax(tmin, B(s));
            }
            if(tmax > E(s)) {
                warning(8013, "(%.5g > %.5g)", tmax, E(s));
                outmsg();
                clrmsg();
                tmax = fmin(tmax, E(s));
            }
            nofsignal = (int)((tmin - B(s)) / DT(s));
            nlnsignal = (int)((tmax - B(s)) / DT(s)) - nofsignal + 1;
        } else {
            nofsignal = 0;
            nlnsignal = s->h->npts;
        }

        /* -- Sum square of data points within measurement window.  Normalize result. */
        sumsqsignal = 0.;
        if(nlnsignal > 0) {
            for (j = nofsignal; j <= (nofsignal + nlnsignal - 1); j++) {
                sumsqsignal += s->y[j] * s->y[j];
            }
            sumsqsignal = sumsqsignal / (double) (nlnsignal);
        } else {
            error(*nerr = 8008, "");
            goto L_8888;
        }

        /* -- Perform same calculation on noise window if requested.
         *    Subtract result from signal summation. */
        if (cmsmm.lnoisemtw) {
            getatw((char *) kmsmm.knoisemtw, 9, cmsmm.onoisemtw, &tmin, &tmax,
                   &nofnoise, &nlnnoise, nerr);
            if (*nerr != 0) {
                goto L_8888;
            }
            tmin = fmax(tmin, B(s));
            tmax = fmin(tmax, E(s));
            nofnoise = (int)((tmin - B(s))/DT(s));
            nlnnoise = (int)((tmax - B(s))/DT(s)) - nofnoise + 1;
            sumsqnoise = 0.;
            for (j = nofnoise; j <= (nofnoise + nlnnoise - 1); j++) {
                sumsqnoise += s->y[j] * s->y[j];
            }
            sumsqnoise = sumsqnoise / (float) (nlnnoise);
            sumsq = sumsqsignal - sumsqnoise;
            if (sumsqnoise > sumsqsignal) {
                setmsg("ERROR", ERROR_RMS_NOISE_GREATER_THAN_SIGNAL);
                outmsg();
                clrmsg();
                sumsq = 0.0;
            }
        } else {
            sumsq = sumsqsignal;
        }

        /* -- Compute the resulting rms value and store in the requested header field. */
        rms = sqrt(sumsq);
        sac_set_float(s, ifpick, rms);
    }                           /* end for ( jdfl ) */

  L_8888:
    return;

}                               /* end of function */
