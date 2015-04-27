
#include "sam.h"
#include "dfm.h"
#include "hdr.h"
#include "amf.h"
#include "bool.h"

#include "ucf.h"
#include "dff.h"

void /*FUNCTION*/
xhan(nerr)
     int *nerr;
{
    int j, jdfl;

    sac *s;
        /*=====================================================================
	 * PURPOSE:  To execute the action command HANNING.
	 *           This command applies a Hanning window to data in memory.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    HDR:     DEPMIN, DEPMAX, DEPMEN
	 *    DFM:     SACMEM()
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  VFLIST, VFEVEN, GETFIL, EXTRMA, PUTFIL
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    /* CHECKING PHASE: */

    /* - Check for null data file list. */

    vflist(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Check to make sure all files are evenly spaced time series files. */

    vfeven(nerr);
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

        /* -- Apply Hanning window to dependent data array.
         *    The two endpoints are defined separately. */

        for (j = 1; j <= s->h->npts - 2; j++) {
            s->y[j] = 0.25 * s->y[j - 1] + 0.5 * s->y[j] + 0.25 * s->y[j + 1];
        }
        s->y[0] = s->y[1];
        s->y[s->h->npts - 1] = s->y[s->h->npts - 2];

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
	 *    820809:  Changed to newest set of parsing and checking functions.
	 *    810224:  Original version.
	 *===================================================================== */

}                               /* end of function */
