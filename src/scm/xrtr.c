
#include <stdio.h>

#include "scm.h"
#include "cpf.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "msg.h"
#include "bbs.h"
#include "ucf.h"
#include "dff.h"

void /*FUNCTION*/
xrtr(nerr)
     int *nerr;
{
    int jdfl;
    float corrcf, sddta, sdslp, sdyint, slp, yint;

    static int verbose = FALSE;
    sac *s;
        /*=====================================================================
	 * PURPOSE:  To execute the action command RTREND.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag.
	 *=====================================================================
	 * MODULE/LEVEL:  SCM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    DFM:     DEPMIN, DEPMAX, DEPMEN
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  VFLIST, VFTIME, GETFIL, LIFITE, LIFITU,
	 *             WRTXTT, EXTRMA, PUTFIL
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    SLP:     Slope of linear trend.
	 *    YINT:    Intercept of linear trend.
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    /* CHECKING PHASE: */

    /* - Check for null data file list. */

    while (lcmore(nerr)) {
        if (lckey("V#ERBOSE$", 10)) {
            verbose = TRUE;
        }
        if (lckey("Q#UIET$", 8)) {
            verbose = FALSE;
        }
    }

    vflist(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Check to make sure all files are time series files. */

    vftime(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* EXECUTION PHASE: */

    /* - Perform the requested function on each file in DFL. */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        /* -- Get the next file in DFL, moving header to CMHDR. */
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        //getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

        /* -- Two versions of linear fit: one for evenly spaced
         *    data and one for unevenly spaced data. */

        if (s->h->leven) {
            lifite(s->h->b, s->h->delta, s->y, s->h->npts, &slp, &yint, &sdslp,
                   &sdyint, &sddta, &corrcf);
            rtrend(s->y, s->h->npts, yint, slp, s->h->b, s->h->delta);
        } else {
            lifitu(s->x, s->y, s->h->npts, &slp, &yint, &sdslp, &sdyint, &sddta,
                   &corrcf);
            rtrend2(s->y, s->h->npts, yint, slp, s->x);
        }

        /* -- Write results oflinear fit. */
        if (verbose) {
            setmsg("OUTPUT", 0);
            out("Slope and standard deviation are: %.5g %.5g\n"
                " Intercept and standard deviation are: %.5g %.5g\n"
                " Data standard deviation is: %.5g\n"
                " Data correlation coefficient is: %.5g",
                slp, sdslp, yint, sdyint, sddta, corrcf);
            outmsg();
        }

        /* -- write the fitting parameters to blackboard variables */
        setbb("rtr_slp ", VAR_VALUE, slp);
        setbb("rtr_sdslp ", VAR_VALUE, sdslp);
        setbb("rtr_yint ", VAR_VALUE, yint);
        setbb("rtr_sdyint ", VAR_VALUE, sdyint);
        setbb("rtr_sddta ", VAR_VALUE, sddta);
        setbb("rtr_corrcf ", VAR_VALUE, corrcf);

        /* -- Update any header fields that may have changed. */

        extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax,
               &s->h->depmen);

    }

    /* - Calculate and set new range of dependent variable. */

    setrng();

  L_8888:
    return;

        /*=====================================================================
	 * MODIFICATION HISTORY:
	 *    820817:  Documented subroutine.
	 *    820817:  Changed to newest set of parsing and checking functions.
	 *    810528:  Original version.
	 *===================================================================== */

}                               /* end of function */
