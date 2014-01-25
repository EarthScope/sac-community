
#include "gtm.h"

void /*FUNCTION*/ getworld(xmin, xmax, ymin, ymax)
float *xmin, *xmax, *ymin, *ymax;
{



	/*=====================================================================
	 * PURPOSE:  To get the world coordinate limits.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    xmin:    Minimum world coordinate value in x direction. [f]
	 *    xmax:    Maximum world coordinate value in y direction. [f]
	 *    ymin:    Minimum world coordinate value in y direction. [f]
	 *    ymax:    Maximum world coordinate value in x direction. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    gtm:     xwcmin, xwcmax, ywcmin, ywcmax
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    903010:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900310
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Return current world coordinates. */
	*xmin = cmgtm.xwcmin;
	*xmax = cmgtm.xwcmax;
	*ymin = cmgtm.ywcmin;
	*ymax = cmgtm.ywcmax;

       
	return;

} /* end of function */

