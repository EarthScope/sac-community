
#include "scm.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "ucf.h"
#include "cpf.h"
#include "dff.h"
#include "defs.h"

SCM_EXTERN

#define	MHALF	128

void /*FUNCTION*/
xsmooth(nerr)
     int *nerr;
{
    int index[2 * MHALF + 1], j, jdfl, jnew, jnew1, jnew2, nfull;
    float factor, sum;

    float *Sacmem, *Sacmem1, *Sacmem2;

    int *const Index = &index[0] - 1;
    sac *s;
    float *new;
        /*=====================================================================
	 * PURPOSE:  To execute the action command SMOOTH.
	 *           This command applies several different smoothing algorithms.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:  1001
	 *=====================================================================
	 * MODULE/LEVEL:  SCM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SCM:     LMEAN, NHALF
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    NDSFILES: Number of files in a given data-set. [i]
	 *    NDSFLNUM: File number (senquental) in the current data set. [i]
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LCLOG, LCLIST, LKIRC
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920317:  Added data-set storage. wct
	 *    870203:  Changed to an action command and moved to SCM.
	 *    820721:  Changed to newest set of parsing and checking functions.
	 *    810120:  Changed to output message retrieval from disk.
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    /* PARSING PHASE: */

    /* - Loop on each token in command: */

  L_1000:
    if (lcmore(nerr)) {

        /* -- "MEAN/MEDIAN":  Change type of smoothing and turn smoothing on. */
        if (lclog2("MEAN$", 6, "MEDIAN$", 8, &cmscm.lmean)) {

            /* -- "HALFWIDTH n":  Change half width of smoothing region. */
        } else if (lkirc("HALFWID#TH$", 12, 1, MHALF, &cmscm.nhalf)) {

            /* -- Bad syntax. */
        } else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();

        }
        goto L_1000;

    }

    /* - The above loop is over when one of two conditions has been met:
     *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
     *   (2) All the tokens in the command have been successfully parsed. */

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

    /* - Perform the requested function on each file in DFL. */

    nfull = 2 * cmscm.nhalf + 1;
    factor = 1. / (float) (nfull);

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {

        /* -- Get next file from the memory manager.
         *    (Header is moved into common blocks CMHDR and KMHDR.) */
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        //getfil( jdfl, TRUE, &nlen, &ndxold, &notused, nerr );

        /* -- Check full width versus number of data points. */

        /* -- Allocate a new block for smoothed data. */
        new = (float *) malloc(sizeof(float) * s->h->npts);

        /* -- Compute start and end points for smoothing. */
        /* jold1 = ndxold + cmscm.nhalf; */
        jnew1 = cmscm.nhalf;
        jnew2 = s->h->npts - 1 - cmscm.nhalf;

        /* -- Perform smoothing on interior points.
         * --- Mean smoothing. */
        if (cmscm.lmean) {
            sum = 0.;
            for (j = 0; j < nfull; j++) {
                sum += s->y[j];
            }
            Sacmem1 = new + cmscm.nhalf;
            *(Sacmem1++) = factor * sum;

            Sacmem2 = s->y + cmscm.nhalf + 1;

            for (jnew = jnew1 + 1; jnew <= jnew2; jnew++) {
                sum =
                    sum + *(Sacmem2 + cmscm.nhalf) - *(Sacmem2 - cmscm.nhalf -
                                                       1);
                *(Sacmem1++) = factor * sum;
                Sacmem2++;
            }
            /* --- Median smoothing. */
        } else {
            Sacmem1 = new + cmscm.nhalf;
            Sacmem2 = s->y + cmscm.nhalf;
            for (jnew = jnew1; jnew <= jnew2; jnew++) {
                srtndx(Sacmem2 - cmscm.nhalf, nfull, index, nerr);
                if (*nerr != 0)
                    goto L_8888;

                *(Sacmem1++) =
                    *(Sacmem2 - cmscm.nhalf + Index[cmscm.nhalf + 1] - 1);
                Sacmem2++;
            }
        }

        /* -- Replicate end points. */
        Sacmem = new + cmscm.nhalf - 1;
        for (jnew = jnew1 - 1; jnew >= (jnew1 - cmscm.nhalf); jnew--) {
            *(Sacmem--) = *(new + cmscm.nhalf);
        }

        Sacmem = new + s->h->npts - cmscm.nhalf;
        for (jnew = jnew2 + 1; jnew <= (jnew2 + cmscm.nhalf); jnew++) {
            *(Sacmem++) = *(new + s->h->npts - 1 - cmscm.nhalf);
        }

        /* -- Update dfl indices and delete old data block. */
        FREE(s->y);
        s->y = new;
        /* update header values */

        extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax,
               &s->h->depmen);

    }

    /* - Calculate and set new range of dependent variable. */

    setrng();

  L_8888:
    return;

}                               /* end of function */
