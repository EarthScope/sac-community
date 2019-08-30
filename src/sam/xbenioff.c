
#include "sam.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "ucf.h"
#include "dff.h"

void /*FUNCTION*/
xbenioff(nerr)
     int *nerr;
{
    int j, jdfl;
    sac *s;

        /*=====================================================================
	 * PURPOSE: To parse and execute the action command BENIOFF.
	 *          This command applies a Benioff filter to data in memory.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  XSC/2
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
	 * MODIFICATION HISTORY:
	 *    870211:  Converted to an internal command.
	 *    841206:  Original XSC version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870211
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

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

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {

        /* -- Get next file from the memory manager.
         *    (Header is moved into common blocks CMHDR and KMHDR.) */
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        //getfil( jdfl, TRUE, &nlen, &ndxy, &ndxx, nerr );

        /* -- Initialize filter for this file. */
        filtb(0, (float)DT(s));

        /* -- Filter this data file. */

        for (j = 0; j < s->h->npts; j++) {
            s->y[j] = (float) filtb(1, s->y[j]);
        }

        /* -- Update any header fields that may have changed. */
        extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax,
               &s->h->depmen);

    }

    /* - Calculate and set new range of dependent variable. */

    setrng();

  L_8888:
    return;

}                               /* end of function */
