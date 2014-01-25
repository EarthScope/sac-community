
#include "gtm.h"

void /*FUNCTION*/ worldtovport(xwloc, ywloc, xloc, yloc)
double xwloc, ywloc;
float *xloc, *yloc;
{



	/*=====================================================================
	 * PURPOSE: To convert a world location to a viewport location.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    xwloc:   X location in world coordinates. [f]
	 *    ywloc:   Y location in world coordinates. [f]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    xloc:    X location in viewport coordinates. [f]
	 *    yloc:    Y location in viewport coordinates. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    gtm:     xmpwv1, xmpwv2, ympwv1, ympwv2
	 *=====================================================================
	 * GLOBAL COUPLING:
	 * - Use setworld and setvport to set world and viewport mapping.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861027:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861027
	 *===================================================================== */
	/* PROCEDURE: */
	*xloc = cmgtm.xmpwv1*xwloc + cmgtm.xmpwv2;
	*yloc = cmgtm.ympwv1*ywloc + cmgtm.ympwv2;

       
	return;

} /* end of function */

