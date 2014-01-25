
#include "gtm.h"

void /*FUNCTION*/ vporttoworld(xloc, yloc, xwloc, ywloc)
double xloc, yloc;
float *xwloc, *ywloc;
{



	/*=====================================================================
	 * PURPOSE: To convert a viewport location to a world location.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    xloc:    X location in viewport coordinates. [f]
	 *    yloc:    Y location in viewport coordinates. [f]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    xwloc:   X location in world coordinates. [f]
	 *    ywloc:   Y location in world coordinates. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    gtm:     xmpwv1, xmpwv2, ympwv1, ympwv2
	 *=====================================================================
	 * GLOBAL COUPLING:
	 * - use setvport and setworld to set viewport and world coordinates.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861027:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861027
	 *===================================================================== */
	/* PROCEDURE: */
	*xwloc = (xloc - cmgtm.xmpwv2)/cmgtm.xmpwv1;
	*ywloc = (yloc - cmgtm.ympwv2)/cmgtm.ympwv1;

       
	return;

} /* end of function */

