
#include "gtm.h"

void /*FUNCTION*/ setvportratio(ratio)
double ratio;
{
	float vpratio;



	/*=====================================================================
	 * PURPOSE:  To define the desired viewport (y to x) aspect ratio.
	 *           Current viewport is modified to maintain this ratio.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    ratio:   Desired aspect ratio. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    gtm:     xvpmin, xvpmax, yvpmin, yvpmax
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gtm:     xvpmin, xvpmax, yvpmin, yvpmax
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:     calwvtransform
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900305:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900305
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Determine the largest viewport values with requested aspect ratio. */
	vpratio = (cmgtm.yvpmax - cmgtm.yvpmin)/(cmgtm.xvpmax - cmgtm.xvpmin);
	if( ratio <= vpratio ){
		cmgtm.yvpmax = cmgtm.yvpmin + ratio*(cmgtm.xvpmax - cmgtm.xvpmin);
		}
	else if( ratio > 0. ){
		cmgtm.xvpmax = cmgtm.xvpmin + (cmgtm.yvpmax - cmgtm.yvpmin)/
		 ratio;
		}

	/* - Calculate the world to viewport mapping transformation. */

	calwvtransform();

       
	return;

} /* end of function */

