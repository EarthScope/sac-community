#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "amf.h"
#include "gam.h"
#include "dfm.h"
#include "hdr.h"
#include "bool.h"

void
getylm(lylmon, ystart, ystop)
     int *lylmon;
     float *ystart, *ystop;
{
    int i;
    sac *s;
        /*=====================================================================
	 * PURPOSE:  To return y axis plot limit attributes for current data file.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    LYLMON:  Set to .TRUE. if fixed y limits option is on. [l]
	 *    YSTART:  Minimum y limit when LYLMON is .TRUE. [f]
	 *    YSTOP:   Maximum y limit when LYLMON is .TRUE. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  GAM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GAM:     KYLIMS, YLIMS, RNGMIN, RNGMAX
	 *    DFM:     IDFLC
	 *    HDR:     DEPMIN, DEPMAX
	 *=====================================================================
	 * GLOBAL COUPLING:
	 * - SETRNG calculates RNGMIN and RNGMAX.
	 * - YLIM command defines KYLIMS and YLIMS.
	 *===================================================================== */
    /* PROCEDURE: */
    /* - Determine proper limits for "current" data file: */
    s = sacget_current();
    i = sacget_current_id();
    DEBUG("i: %d\n", i);
    if (i < 0) {
        return;
    }
    DEBUG("i: %d <%s>\n", i, kmgam.kylims[i]);
    if (strcmp(kmgam.kylims[i], "ON      ") == 0) {

        /* -- limits set to fixed values. */
        *lylmon = TRUE;
        *ystart = cmgam.ylims[i][0];
        *ystop = cmgam.ylims[i][1];

        /* -- limits set to range of entire data file list. */
    } else if (strcmp(kmgam.kylims[i], "ALL     ") == 0) {
        *lylmon = TRUE;
        *ystart = cmgam.rngmin;
        *ystop = cmgam.rngmax;
        DEBUG("ALL: %f %f\n", cmgam.rngmin, cmgam.rngmax);
        /* -- limits not fixed; plot will be scaled to data itself. */
    } else {
        *lylmon = FALSE;
        *ystart = s->h->depmin;
        *ystop = s->h->depmax;
    }

    return;

        /*=====================================================================
	 * MODIFICATION HISTORY:
	 *    810709:  Modifications for YLIM ALL option.
	 *    810203:  Original version.
	 *===================================================================== */

}                               /* end of function */
