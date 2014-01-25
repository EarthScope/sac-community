
#include "gtm.h"

void /*FUNCTION*/ setvport(xmin, xmax, ymin, ymax)
double xmin, xmax, ymin, ymax;
{



	/*=====================================================================
	 * PURPOSE:  To set the viewport coordinate plot limits.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    xmin:    Minimum viewport coordinate value in x direction. [f]
	 *    xmax:    Maximum viewport coordinate value in y direction. [f]
	 *    ymin:    Minimum viewport coordinate value in y direction. [f]
	 *    ymax:    Maximum viewport coordinate value in x direction. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gtm:     xvpmin, xvpmax, yvpmin, yvpmax
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  calwvtransform
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861027:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861027
	 *===================================================================== */
	/* - Save requested viewport. */
	cmgtm.xvpmin = xmin;
	cmgtm.xvpmax = xmax;
	cmgtm.yvpmin = ymin;
	cmgtm.yvpmax = ymax;

	/* - Calculate world/viewport mapping transformation. */

	calwvtransform();

       
	return;

} /* end of function */

