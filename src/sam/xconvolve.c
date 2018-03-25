
#include <string.h>

#include "sam.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "co.h"
#include "ucf.h"
#include "msg.h"
#include "clf.h"
#include "dbh.h"
#include "cpf.h"
#include "dff.h"

#define	MCORLN	4096
#define	MWINLN	2048
SAM_EXTERN

void /*FUNCTION*/
xconvolve(nerr)
     int *nerr;
{
    char kermsg[131], ktemp1[MCPFN + 1];

    int iwinln, iwinmx, j, jdfl, nfft, nzeros, nlenmx,  /* max npts of all signals */
      nlenMaster,               /* npts of master */
      nlenCombined;             /* nlen + nlenMaster - 1 */
    float *master, *correl, *signal;
    sac *s;
    float *destination, *source;        /* reverse and copy master. maf 961204 */

        /*=====================================================================
	 * PURPOSE: To parse and execute the action command CONVOLVE.
	 *          This command computes convolutions.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    HDR:     DEPMIN, DEPMAX, DEPMEN
	 *    MEM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    MWINLN:  Maximum length of each data window. [ip]
	 *    MCORLN:  Maximum length of correlation function. [ip]
	 *    NDXMAS:  Index in SACMEM array for master signal. [i]
	 *    NDXSIG:  Index in SACMEM array for current signal. [i]
	 *    NDXCOR:  Index in SACMEM array for unshifted correlation. [i]
	 *    NDSFILES: Number of files in a given data-set. [i]
	 *    NDSFLNUM: File number (senquental) in the current data set. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    961204:  Modified to return full range of values instead of just
	 *             the central half.  Timing changed to leave begin times
	 *             of signals unchanged from the input signals.  
	 *    920110:  Added DATA-SET update logic.
	 *    870925:  Fixed bug when signals were of different length.
	 *             Now output signals are all equal in length to the
	 *               maximum length of the input signals.
	 *    870312:  Added ability to choose master file by name.
	 *    870209:  Converted to an internal command.
	 *    830000:  Original XSC version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870925
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    /* - PARSING PHASE: */

    /* - Loop on each token in command: */

    while (lcmore(nerr)) {

        /* -- "MASTER name|n":  determine which file to copy from. */
        if (lckey("MASTER$", 8)) {
            if (lcirc(1, saclen(), &cmsam.imast)) {     /* do nothing */
            } else if (lcchar(ktemp1, sizeof(ktemp1))) {
                char *ktemp2 = fstrdup(ktemp1, -1);
                if ((cmsam.imast = sac_find_filename(ktemp2)) < 0) {
                    arg_prev();
                    cfmt("BAD FILE NAME:", 16);
                    cresp();
                }
                cmsam.imast += 1;
            } else {
                cfmt("NEED A FILE NAME OR A NUMBER:", 31);
                cresp();
            }                   /* end else */

            ;

        }

        /* end if ( lckey ( "MASTER$" , 8 ) ) */
        /* -- "NUMBER n":  set number of windows. */
        else if (lkint("NUMBER$", 8, &cmsam.nwin)) {    /* do nothing */
        } else if (lklog("AMP#LITUDE$", 10, &cmsam.amplitude)) {
        }

        /* -- "LENGTH ON|OFF|v":  set window length in seconds. */
        else if (lklogr("LENGTH$", 8, &cmsam.lwinln, &cmsam.winln)) {
            if (cmsam.winln <= 0.)
                cmsam.lwinln = FALSE;
        }

        /* end else if( lklogr( "LENGTH$" ... */
        /* -- "TYPE char":  set window (taper) type. */
        else if (lklist("TYPE$", 6, (char *) kmsam.kwintp, 9, MWINTP, &cmsam.iwintp)) { /* do nothing */
        }

        /* -- Bad syntax. */
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();

        }                       /* end else */
    }                           /* end while */

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

    /* - Find longest signal. */

    nlenmx = 0;
    iwinmx = 0;
    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            *nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
            goto L_8888;
        }
        //getfil( jdfl, FALSE, &ntused, &ntused, &ntused, nerr );
        if (*nerr != 0)
            goto L_8888;
        if (cmsam.lwinln) {
            iwinln = (int) (cmsam.winln / s->h->delta + 0.1);
        } else {
            iwinln = s->h->npts / cmsam.nwin;
        }

        nlenmx = max(nlenmx, s->h->npts);
        iwinmx = max(iwinmx, iwinln);
    }                           /* end for */

    /* - EXECUTION PHASE: */

    /* - Allocate temporary blocks for the master signal and correlation function. */

    master = (float *) malloc(sizeof(float) * nlenmx);
    nfft = 8;

    while (nfft < (2 * iwinmx - 1))
        nfft *= 2;
    correl = (float *) malloc(sizeof(float) * nfft);

    /* - Get the master signal, reverse and copy to first temporary block.
     *   Pad with zeros if necessary. */

    /* nlen became nlenMaster.  maf 961204 */
    if (!(s = sacget(cmsam.imast - 1, TRUE, nerr))) {
        *nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
        goto L_7777;
    }
    //getfil( cmsam.imast, TRUE, &nlenMaster, &ndxy, &ndxx, nerr );
    nlenMaster = s->h->npts;

    /* clone and reverse the master signal. overhauled, maf 961204 */
    destination = master;
    source = s->y + s->h->npts - 1;

    while (source >= s->y) {
        *destination = *source;
        destination++;
        source--;
    }

    /* pad with zeros if necessary. */
    nzeros = nlenmx - nlenMaster;       /* nlen became nlenMaster.  maf 961204 */
    if (nzeros > 0)
        fill(master + nlenMaster, nzeros, 0.);

    /* - Perform the requested function on each file in DFL. */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {

        /* -- Get next file from the memory manager.
         *    (Header is moved into common blocks CMHDR and KMHDR.) */
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_7777;
        }
        //getfil( jdfl, TRUE, &nlen, &ndxy, &ndxx, nerr );

        nlenCombined = s->h->npts + nlenMaster - 1;     /* added. maf 961204 */

        /* -- Allocate a new block, copy signal to it, and pad with zeros if necessary. */
        signal = (float *) malloc(sizeof(float) * 2 * nlenmx);
        copy_float(s->y, signal, s->h->npts);
        nzeros = 2 * nlenmx - s->h->npts;
        if (nzeros > 0)
            fill(signal + s->h->npts, nzeros, 0.);

        /* -- Update dfl indices to point to this new block and release old one. */
        //Nlndta[jdfl] = nlenCombined ; /* nlenmx became nlenCombined. maf 961204 */
        //cmdfm.ndxdta[jdfl_][0] = ndxsig;

        /* -- Compute length of each window. */
        if (cmsam.lwinln) {
            iwinln = (int) (cmsam.winln / s->h->delta + 0.1);
        } else {
            iwinln = nlenmx / cmsam.nwin;
        }

        /* -- Compute the (unshifted) correlation. */
        crscor(master, signal, nlenmx, cmsam.nwin, iwinln,
               (char *) kmsam.kwintp[cmsam.iwintp - 1], correl, &nfft, kermsg,
               131);
        if (memcmp(kermsg, "        ", 8) != 0) {
            *nerr = 1;
            setmsg("ERROR", *nerr);
            apcmsg(kermsg, 131);
            goto L_7777;        /* L_8888 became L_7777.  maf 961204 */
        }

        /* -- Perform a circular shift to align the correlation in the output block. */
        /*      overhauled to get full range of convolution.  maf 961204 */
        for (j = 0; j <= nlenMaster - 2; j++)
            signal[j] = correl[nfft - nlenMaster + j + 1];
        for (j = 0; j <= s->h->npts - 1; j++)
            signal[nlenMaster + j - 1] = correl[j];

        /* Pad with zeros if necessary.  maf 961204 */
        nzeros = 2 * nlenmx - 1 - nlenCombined;
        if (nzeros > 0)
            fill(signal + nlenCombined, nzeros, 0.);

        /* -- Update any header fields that may have changed. */
        /*      overhauled to preserve differences in begin times. maf 961204 */
        s->h->npts = nlenCombined;
        sac_be(s);
        if (cmsam.amplitude) {
            for (j = 0; j < s->h->npts; j++) {
                signal[j] = s->h->delta * signal[j];
            }
        }

        extrma(signal, 1, nlenCombined, &s->h->depmin, &s->h->depmax,
               &s->h->depmen);
        FREE(s->y);
        s->y = signal;
        /*          *nzyear = SAC_FLOAT_UNDEFINED ; */
/*		*nzhour = SAC_FLOAT_UNDEFINED ; */

    }                           /* end for(jdfl) */

    /* - Release temporary blocks. */
  L_7777:
    FREE(master);
    FREE(correl);

    /* - Calculate and set new range of dependent variable. */

    setrng();

  L_8888:
    return;

}                               /* end of function */
