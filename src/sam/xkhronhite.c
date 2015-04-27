
#include "sam.h"
#include "dfm.h"
#include "hdr.h"
#include "amf.h"
#include "bool.h"

#include "ucf.h"
#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/
xkhronhite(nerr)
     int *nerr;
{
    int j, jdfl;

    sac *s;

        /*=====================================================================
	 * PURPOSE: To parse and execute the action command KHRONHITE.
	 *          This command applies a Khronhite filter to data in memory.
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
	 *    SAM:     CUTKHR
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

    /* PARSING PHASE: */

    /* - Loop on each token in command: */

  L_1000:
    if (lcmore(nerr)) {

        /* -- "v":  set cutkhr frequency. */
        if (lcreal(&cmsam.cutkhr)) {

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

    /* - Perform the requested function on each file in DFL. */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        //getfil( jdfl, TRUE, &nlen, &ndxy, &ndxx, nerr );

        /* -- Initialize filter for this file. */
        filtk(0, cmsam.cutkhr, s->h->delta);

        /* -- Filter this data file. */
        for (j = 0; j < s->h->npts; j++) {
            s->y[j] = filtk(1, cmsam.cutkhr, s->y[j]);
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
