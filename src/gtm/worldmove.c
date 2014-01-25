#include "gtm.h"

#include "gdm.h"

#define	IABOVE	8
#define	IBELOW	4
#define	ILEFT	1
#define	INSIDE	0
#define	IRIGHT	2

void worldmove(double xwloc, double ywloc)
{
	int iloc;
	float xloc, yloc;

	/*=====================================================================
	 * PURPOSE:  To move to the requested world point.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    xwloc:   X location in world coordinates. [f]
	 *    ywloc:   Y location in world coordinates. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    gtm:     xmpwv1, xmpwv2, ympwv1, ympwv2, lvpclip, xvp, yvp
	 *=====================================================================
	 * GLOBAL COUPLING:
	 * - Use setworld and setvport to set world and viewport.
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *   saclib:   move
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Transform world point to viewport coordinates. */
	xloc = cmgtm.xmpwv1*xwloc + cmgtm.xmpwv2;
	yloc = cmgtm.ympwv1*ywloc + cmgtm.ympwv2;
	iloc = INSIDE;

	/* - If viewport clipping is off, send move to active devices. */

	if( !cmgtm.lvpclip ){
		move( xloc, yloc );

		/* - If viewport clipping is on: */

		}
	else{

		/* -- Determine location of data point relative to viewport. */
		if( yloc > Yvp[2] ){
			iloc = iloc + IABOVE;
			}
		else if( yloc < Yvp[1] ){
			iloc = iloc + IBELOW;
			}
		if( xloc > Xvp[2] ){
			iloc = iloc + IRIGHT;
			}
		else if( xloc < Xvp[1] ){
			iloc = iloc + ILEFT;
			}

		/* -- Move to location if inside viewport. */
		if( iloc == INSIDE ){
			move( xloc, yloc );
			}

		}

	/* - Save current viewport coordinates. */

	cmgtm.xvpold = xloc;
	cmgtm.yvpold = yloc;
	cmgtm.ivpold = iloc;

       
	return;

} /* end of function */

