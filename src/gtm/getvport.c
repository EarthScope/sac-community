
#include "gtm.h"

void /*FUNCTION*/ getvport(xmin, xmax, ymin, ymax)
float *xmin, *xmax, *ymin, *ymax;
{



	/*=====================================================================
	 * PURPOSE:  To get the viewport coordinate plot limits.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    xmin:    Minimum viewport coordinate value in x direction. [f]
	 *    xmax:    Maximum viewport coordinate value in y direction. [f]
	 *    ymin:    Minimum viewport coordinate value in y direction. [f]
	 *    ymax:    Maximum viewport coordinate value in x direction. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    gtm:     xvpmin, xvpmax, yvpmin, yvpmax
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    870202:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870202
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Return current viewport coordinates. */
	*xmin = cmgtm.xvpmin;
	*xmax = cmgtm.xvpmax;
	*ymin = cmgtm.yvpmin;
	*ymax = cmgtm.yvpmax;

       
	return;

} /* end of function */

