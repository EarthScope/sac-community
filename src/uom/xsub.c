
#include <fern/array.h>

#include "uom.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/
xsub(int *nerr)
{
    int j, jdfl, n, new_list;
    double con;

    static double *v = NULL;
    sac *s;

    /* Ind
     *=====================================================================
     * PURPOSE:  To execute the action command SUB.
     *           This command subtracts a constant from data in memory.
     *=====================================================================
     * OUTPUT ARGUMENTS:
     *    NERR:    Error flag. Set to 0 if no error occurred.
     *             Potential error numbers:
     *=====================================================================
     * MODULE/LEVEL:  UOM/2
     *=====================================================================
     * GLOBAL INPUT:
     *    MACH:
     *    DFM:     NDFL
     *    UOM:     CONSUB()
     *=====================================================================
     * GLOBAL OUTPUT:
     *    UOM:     CONSUB()
     *    HDR:     DEPMIN, DEPMAX, DEPMEM
     *=====================================================================
     * SUBROUTINES CALLED:
     *    SACLIB:  LCMORE, LCREAL, CFMT, CRESP,
     *             VFLIST, VFTIME, GETFIL, PUTFIL
     *=====================================================================
     * LOCAL VARIABLES:
     *    CON:     Constant currently being used in subtraction.
     *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    /* PARSING PHASE: */
    new_list = TRUE;
    if (!v) {
        v = xarray_new('d');
    }
    /* - Loop on each token in command: */

  L_1000:
    if (lcmore(nerr)) {

        /* -- "v":  constant to subtract. */
        if (lcreal(&con)) {
            if (new_list) {
                xarray_clear(v);
                new_list = FALSE;
            }
            v = xarray_append(v, con);
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

    /* - Check for null data file list. */

    vflist(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Check to make sure all files are time series files. */

    vftime(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* EXECUTION PHASE: */
    con = 0.0;
    n = xarray_length(v);
    for (jdfl = 1; jdfl <= saclen(); jdfl++) {

        /* -- Get next file from memory manager. */
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        //getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

        /* -- Subtract appropriate constant from each data point. */
        if (jdfl - 1 < n) {
            con = v[jdfl - 1];
        }
        for (j = 0; j < s->h->npts; j++) {
            s->y[j] -= con;
        }

        /* -- Recompute extrema. */
        s->h->depmen = s->h->depmen - con;
        s->h->depmin = s->h->depmin - con;
        s->h->depmax = s->h->depmax - con;

    }

    /* - Calculate and set new range of dependent variable. */

    setrng();

  L_8888:
    return;

        /*=====================================================================
	 * MODIFICATION HISTORY:
	 *    820701:  Documented subroutine.
	 *    820701:  Changed to newest set of parsing and checking functions.
	 *===================================================================== */

}                               /* end of function */
