
#include "uom.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "ucf.h"
#include "cpf.h"
#include "dff.h"

UOM_EXTERN

void /*FUNCTION*/
xdif(nerr)
     int *nerr;
{
    int jdfl;
    sac *s;

        /*=====================================================================
	 * PURPOSE: To parse and execute the action command DIF.
	 *          This command differentiates data in memory.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  UOM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    UOM:     KDIFTP, IDIFTP, NDIFNF
	 *    DFM:     NDFL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    UOM:     MDIFTP, KDIFTP, IDIFTP, NDIFTP
	 *    HDR:     DEPMIN, DEPMAX, DEPMEN
	 *    MEM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CMFT, CRESP, LCLIST, VFLIST, VFEVEN,
	 *             GETFIL, DIF3, DIF5, EXTRMA, PUTFIL
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    /* - PARSING PHASE: */

    /* - Loop on each token in command: */

    while (lcmore(nerr)) {

        /* -- "TWO/THREE/FIVE":  select type of differentiator */
        if (lclist((char *) kmuom.kdiftp, 9, cmuom.ndiftp, &cmuom.idiftp)) {    /* do nothing */
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

    /* - EXECUTION PHASE: */

    /* - Perform the requested function on each file in DFL. */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        /* -- Get the next file in DFL, moving header to CMHDR. */
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        //getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

        /* -- Call the specific subroutine to work on this file. */

        if (cmuom.idiftp == 1) {
            dif2(s->y, s->h->npts, s->h->delta, s->y);
            s->h->npts = s->h->npts - 1;
            s->h->b = s->h->b + 0.5 * s->h->delta;
        } else if (cmuom.idiftp == 2) {
            dif3(s->y, s->h->npts, s->h->delta, s->y);
            s->h->npts = s->h->npts - 2;
            s->h->b = s->h->b + s->h->delta;
        } else {
            dif5(s->y, s->h->npts, s->h->delta, s->y);
            s->h->npts = s->h->npts - 2;
            s->h->b = s->h->b + s->h->delta;
        }

        sac_be(s);

        /* -- Update any header fields that may have changed. */

        if (s->h->idep == IDISP) {
            s->h->idep = IVEL;
        } else if (s->h->idep == IVEL) {
            s->h->idep = IACC;
        } else {
            s->h->idep = IUNKN;
        }
        extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax,
               &s->h->depmen);

    }

    /* - Calculate and set new range of dependent variable. */

    setrng();

  L_8888:
    return;

        /*=====================================================================
	 * MODIFICATION HISTORY:
	 *    831020:  Original version (from XSC DIF and DIFF.)
	 *===================================================================== */

}                               /* end of function */
