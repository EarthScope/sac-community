
#include <math.h>

#include "uom.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "ucf.h"
#include "dff.h"

void /*FUNCTION*/
xabs(nerr)
     int *nerr;
{
    int j, jdfl;

    sac *s;
        /*=====================================================================
	 * PURPOSE:  To execute the action command ABS.
	 *           This command takes the absolute value of data in memory.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag.
	 *=====================================================================
	 * MODULE/LEVEL:  UOM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *    MEM:     SACMEM()
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    MEM:     SACMEM()
	 *    HDR:     DEPMIN, DEPMAX, DEPMIN
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  VFLIST, VFTIME, GETFIL, EXTRMA, PUTFIL
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    /* CHECKING PHASE: */

    /* - Check for null data file list. */

    vflist(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Check to make sure all files are time series files. */

    vftime(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* EXECUTION PHASE: */

    /* - Peform the requested function on each file in DFL. */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {

        /* -- Get the next file in DFL, moving header to CMHDR. */
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        //getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

        /* -- Take absolute value of dependent array. */
        for (j = 0; j < s->h->npts; j++) {
            s->y[j] = fabs(s->y[j]);
        }

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
	 *    820701:  Documented subroutine.
	 *    810224:  Original version.
	 *===================================================================== */

}                               /* end of function */
