/** 
 * @file   linkcontsegs.c
 * 
 * @brief  Link a pair of points
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"
#include "bool.h"

void 
linkcontsegs(level, point1, point2)
int level;
float point1[], point2[];
{
        int done;
	int jaction, jactionstart, jactionstop, jlevel, jlink, jlinkstart, 
	 jlinkstop, jpoint1, jpoint2, jsegment, jstart, jstop;
	float pointstart[2], pointstop[2];

	/*=====================================================================
	 * PURPOSE:  To link a pair of points with a contour segment.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *     level:  Contour segment level index number. [i]
	 *    point1:  First point. [fa]
	 *             point1(1) is x value, point1(2) is y value.
	 *    point2:  Second point. [fa]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    contouring:  MACTIONMOVE, MACTIONDRAW, MLINKOPEN, MLINKCLOSED
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:     GetContSeg, GetContPoint, PutContSeg, PutContPoint, 
	 *             PointsEqual, NewContSeg, NewContPoint
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900405:  Added action attribute and changed link use.
	 *    900322:  Deleted some of the logic due to improvements in the
	 *             way I was calculating the pair of points.
	 *    900315:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900315
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Loop until:
	 *   (1) all segments have been processed OR
	 *   (2) this pair of points has been associated with a segment. */
	done = FALSE;
	jsegment = 0;
L_1000:
	if( nextcontseg( &jsegment, &jlevel, &jstart, &jstop ) && !done
	  ){

		/* -- Get segment start and stop points. */
		getcontpoint( jstart, pointstart, &jlinkstart, &jactionstart );
		getcontpoint( jstop, pointstop, &jlinkstop, &jactionstop );

		/* -- If input level is equal to segment level 
		 *    and segment is not a closed segment (closed contour): */

		if( level == jlevel && (jlinkstop != MLINKCLOSED) ){

			/* --- If first point is the same as stop point, append second point to segment.
			 *     Also if second point same as start point, close segment. */
			if( pointsequal( point1, pointstop ) ){
				if( pointsequal( point2, pointstart ) ){
					jlink = MLINKCLOSED;
					}
				else{
					jlink = MLINKOPEN;
					}
				jaction = MACTIONDRAW;
				newcontpoint( point2, jlink, jaction, &jpoint2 );
				jlink = jpoint2;
				putcontpoint( jstop, pointstop, jlink, jactionstop );
				putcontseg( jsegment, jlevel, jstart, jpoint2 );
				done = TRUE;

				/* --- Repeat (similiar) logic, checking second point versus start point. */
				}
			else if( pointsequal( point2, pointstart ) ){
				if( pointsequal( point1, pointstop ) ){
					jlink = MLINKCLOSED;
					putcontpoint( jstop, pointstop, jlink, jactionstop );
					}
				jlink = jstart;
				jaction = MACTIONMOVE;
				newcontpoint( point1, jlink, jaction, &jpoint1 );
				jaction = MACTIONDRAW;
				putcontpoint( jstart, pointstart, jlinkstart, jaction );
				putcontseg( jsegment, jlevel, jpoint1, jstop );
				done = TRUE;
				}
			}
		goto L_1000;
		}

	/* - If points have still not been associated with a segment,
	 *   start a new segment with this pair. */

	if( !done ){
		jlink = MLINKOPEN;
		jaction = MACTIONDRAW;
		newcontpoint( point2, jlink, jaction, &jpoint2 );
		jlink = jpoint2;
		jaction = MACTIONMOVE;
		newcontpoint( point1, jlink, jaction, &jpoint1 );
		newcontseg( level, jpoint1, jpoint2, &jsegment );
		}
	return;
}

