
#include "gtm.h"

void /*FUNCTION*/ setworld(xwmin, xwmax, ywmin, ywmax)
double xwmin, xwmax, ywmin, ywmax;
{



	/*=====================================================================
	 * PURPOSE:  To set the world coordinate plot limits.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    xwmin:   Minimum world coordinate value in x direction. [f]
	 *    xwmax:   Maximum world coordinate value in y direction. [f]
	 *    ywmin:   Minimum world coordinate value in y direction. [f]
	 *    ywmax:   Maximum world coordinate value in x direction. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gtm:     xwcmin, xwcmax, ywcmin, ywcmax
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  calwvtransform
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861027:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861027
	 *===================================================================== */
	/* - Save requested world coordinates. */
	cmgtm.xwcmin = xwmin;
	cmgtm.xwcmax = xwmax;
	cmgtm.ywcmin = ywmin;
	cmgtm.ywcmax = ywmax;

	/* - Calculate world/viewport mapping transformation. */

	calwvtransform();

       
	return;

} /* end of function */

