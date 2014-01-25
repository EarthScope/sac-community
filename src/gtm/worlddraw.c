#include "gtm.h"
#include "gdm.h"

#include "bot.h"

#define	IABOVE	8
#define	IBELOW	4
#define	ILEFT	1
#define	INSIDE	0
#define	IRIGHT	2

void worlddraw(double xwloc, double ywloc)
{
	int iloc, ixyloc[2], n;
	float xline[2], xloc, yline[2], yloc;

	int *const Ixyloc = &ixyloc[0] - 1;
	float *const Xline = &xline[0] - 1;
	float *const Yline = &yline[0] - 1;


	/*=====================================================================
	 * PURPOSE:  To draw from the current point to the requested world point.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    xwloc:   X world location. [f]
	 *    ywloc:   Y world location. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    gtm:     xmpwv1, xmpwv2, ympwv1, ympwv2, lvpclip, xvp, yvp,
	 *             xvpold, yvpold, ivpold
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *   saclib:   draw, move, clipdp
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *   xline:    X locations of line from current point to input point.
	 *   yline:    Y locations of line from current point to input point.
	 *   ixyloc:   Locations of line segment relative to viewport.
	 *   iabove:   Value indicating a point is above the viewport.
	 *   ibelow:   Value indicating a point is below the viewport.
	 *   iright:   Value indicating a point is to right of viewport.
	 *   ileft:    Value indicating a point is to left of viewport.
	 *   inside:   Value indicating a point is inside viewport.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *   870501:   Added optional viewport clipping.
	 *   830523:   Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870501
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Transform from world to viewport coordinates. */
	xloc = cmgtm.xmpwv1*xwloc + cmgtm.xmpwv2;
	yloc = cmgtm.ympwv1*ywloc + cmgtm.ympwv2;
	iloc = INSIDE;

	/* - If viewport clipping is off,  perform draw. */

	if( !cmgtm.lvpclip ){

		draw( xloc, yloc );
                stroke();
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

		/* -- There are three cases to handle:
		 *    (1) Entire line segment is inside viewport:
		 *        Move has already been done. Do draw. */

		if( (cmgtm.ivpold + iloc) == INSIDE ){
			draw( xloc, yloc );
                        stroke();
			/*    (2) Part of line segment is inside viewport:
			 *        Clip line segment to viewport. Do move and draw. */

			}
		else if( ( cmgtm.ivpold & iloc ) == 0 ){
			Xline[1] = cmgtm.xvpold;
			Xline[2] = xloc;
			Yline[1] = cmgtm.yvpold;
			Yline[2] = yloc;
			Ixyloc[1] = cmgtm.ivpold;
			Ixyloc[2] = iloc;
			clipdp( xline, yline, ixyloc, xvp, yvp, &n );

			move( Xline[1], Yline[1] );
			draw( Xline[2], Yline[2] );
                        stroke();
			}

		/*    (3) Entire line segment is outside viewport: Do nothing. */

		}

	/* - Save current viewport coordinates. */

	cmgtm.xvpold = xloc;
	cmgtm.yvpold = yloc;
	cmgtm.ivpold = iloc;

       
	return;

} /* end of function */

