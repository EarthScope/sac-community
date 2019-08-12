
#include <string.h>

#include "sam.h"
#include "amf.h"
#include "dfm.h"
#include "hdr.h"
#include "bool.h"

#include "msg.h"
#include "clf.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"
#include "defs.h"

SAM_EXTERN

void /*FUNCTION*/
xunwr(nerr)
     int *nerr;
{
    int lok;
    int int_, j, jdfl, jj, nfreq, nlnaux, nlnnew, nok, nptsmx;
    double scalef;

    char *tmp;
    sac *s;
    float *x, *y, *aux1, *aux2, *aux3;
        /*=====================================================================
	 * PURPOSE: To parse and execute the action command UNWRAP.
	 *          This command does a phase unwrapping.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    HDR:     IAMPH
	 *    SAM:     MFFT
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    DFM:     NDXDTA, NLNDTA, NCOMP
	 *    HDR:     NSNPTS, NPTS, IFTYPE, SB, SDELTA, B, DELTA, E
	 *    SAM:     LUNWFZ, NUNWFZ, VUNWIT, VUNWCT
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, LCLOG2, CFMT, CRESP, VFLIST, VFEVEN, VFMAXN,
	 *             GETFIL, ALLAMB, COPY, FILL, RELAMB, UNWRAP, TOAMPH,
	 *             PUTFIL, SETMSG, APIMSG, APCMSG, OUTMSG, NEXT2
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    NPTSMX:  Maximum number of points in files in DFL. [i]
	 *    NDXAUX1: Index in SACMEM array of first scratch array. [i]
	 *    NDXAUX2: Index in SACMEM array of second scratch array. [i]
	 *    NDXAUX3: Index in SACMEM array of third scratch array. [i]
	 *    NLNOLD:  Number of points in current file before transform.. [i]
	 *    NDXOLD:  Index in SACMEM array of time-series before transform. [i]
	 *    NLNNEW:  Number of points in current file after transform. [i]
	 *    NDX1:    Index in SACMEM of first component after transform. [i]
	 *    NDX2:    Index in SACMEM of second component after transform. [i]
	 *    NFREQ:   Number of frequencies in transformed data. [i]
	 *    SCALEF:  Scaling factor applied after transform. [f]
	 * Added for data-set storage.
	 *    NDSFILES: Number of files in a given data-set. [i]
	 *    NDSFLNUM: File number (senquental) in the current data set. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    911015:  Added data-set storage, two sections of code.
	 *    910320:  lok=.false. returned by unwrap changed from WARN to ERR.
	 *    910215:  Debug: remove scaling of phase result (jy)
	 *    870727:  Use memory manager for scratch space rather than fixed
	 *             size arrays inside of unwrap.
	 *    850617:  Major rewrite due to addition of new memory manager.
	 *    820817:  Changed to newest set of parsing and checking functions.
	 *    810806:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850617
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    /* - PARSING PHASE: */

    /* - For each token in command: */

    int_ = 0;
    while (lcmore(nerr)) {

        /* -- "FILL n/OFF/ON":  fill with zeros option. */
        if (lklogi("FILL$", 6, &cmsam.lunwfz, &int_)) {
            int_ = next2(int_);
            if (int_ <= MFFT)
                cmsam.nunwfz = int_;

            else {
                int_ = MFFT;
                *nerr = 1606;
                setmsg("ERROR", *nerr);
                apimsg(int_);
                goto L_8888;
            }
        }

        /* -- "INCTHR v":  phase increment threshold parameter. */
        else if (lkreal("INTTHR$", 8, &cmsam.vunwit)) { /* do nothing */
        }

        /* -- "CONTHR v":  phase consistency threshold parameter. */
        else if (lkreal("PVTHR$", 7, &cmsam.vunwct)) {  /* do nothing */
        }

        /* -- Bad syntax. */
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }

    }                           /* end while */

    /* - The above loop is over when one of two conditions has been met:
     *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
     *   (2) All the tokens in the command have been successfully parsed. */

    if (*nerr != 0)
        goto L_8888;

    /* CHECKING PHASE: */

    /* - Check for null data file list. */

    vflist(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Check to make sure all files are evenly spaced time series files. */

    vfeven(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* get the maximum number of points in the input files */

    vfmax(&nptsmx, nerr);
    if (*nerr != 0)
        goto L_8888;

    /* EXECUTION PHASE: */

    /* - Allocate three scratch arrays for use by unwrap.
     *   Each must be equal to the size of the fft being performed. */

    if (cmsam.lunwfz)
        nlnaux = cmsam.nunwfz;

    else
        nlnaux = next2(nptsmx);

    aux1 = (float *) malloc(sizeof(float) * nlnaux);
    aux2 = (float *) malloc(sizeof(float) * nlnaux);
    aux3 = (float *) malloc(sizeof(float) * nlnaux);
    if (!aux1 || !aux2 || !aux3) {
        goto L_8888;
    }

    /* - For each file in data file list: */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        /* -- Get file from memory manager. */
        //getfil( jdfl, TRUE, &nlnold, &ndxold, &ntused, nerr );

        /* - Compute length of data after transform. */
        if (cmsam.lunwfz) {
            nlnnew = cmsam.nunwfz;
        } else {
            nlnnew = next2(s->h->npts);
        }

        /* - Allocate memory block for first component. */
        y = (float *) malloc(sizeof(float) * nlnnew);

        /* -- Copy time-series data into first block and zero fill. */
        copy_float(s->y, y, s->h->npts);
        fill(y + s->h->npts, nlnnew - s->h->npts, 0.);

        /* -- Release old data block containing time-series. */
        FREE(s->y);
        s->y = y;

        /* -- Allocate memory block for second component and zero fill. */
        x = (float *) malloc(sizeof(float) * nlnnew);
        fill(x, nlnnew, 0.);
        FREE(s->x);
        s->x = x;
        /* -- Perform phase unwrapping. */
        unwrap(s->y, s->h->npts, nlnnew, (float) cmsam.vunwct,
               (float) cmsam.vunwit, aux1, aux2, aux3, s->y, s->x, &nok, &lok);

        /* -- Check for errors. */
        if (!lok) {
            *nerr = 1610;
            setmsg("ERROR", 1610);
            apimsg(nok);
            tmp = s->m->filename;
            apcmsg2(tmp, strlen(tmp) + 1);
            outmsg();
            clrmsg();
            goto L_8888;
        }

        /* -- Scale the transformed data. */
        nfreq = nlnnew / 2;
        scalef = DT(s);

        s->y[0] *= scalef;
        s->y[nfreq] *= scalef;
        for (j = 1; j <= (nfreq - 1); j++) {
            s->y[j] *= scalef;
            jj = nlnnew - j;
            s->y[jj] = s->y[j];
            s->x[jj] = -s->x[j];
        }

        /* -- Write DC level to terminal. */
        setmsg("OUTPUT", 1607);
        if (s->x[0] != 0) {
            apfmsg(-s->y[0]);
        } else {
            apfmsg(s->y[0]);
        }
        outmsg();
        clrmsg();

        /* -- Adjust header to reflect new status. */
        s->h->nsnpts = s->h->npts;
        s->h->npts = nlnnew;
        s->h->iftype = IAMPH;
        sac_set_float(s, SAC_SB, B(s));
        sac_set_float(s, SAC_SDELTA, DT(s));
        sac_set_float(s, SAC_B, 0.0);
        sac_set_float(s, SAC_DELTA, 1. / (DT(s) * (double) (s->h->npts)));
        sac_be(s);
    }

    /* - Release scratch space. */
    FREE(aux1);
    FREE(aux2);
    FREE(aux3);

    /* - Calculate and set new range of dependent variable. */

    setrng();

  L_8888:
    return;

}                               /* end of function */
