/** 
 * @file   mergecontsegs.c
 * 
 * @brief  Merge segments
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"
#include "bool.h"

void 
mergecontsegs()
{
	int again, merged;
	int jactionstarta, jactionstartb, jactionstopa, jactionstopb, 
        jlevela, jlevelb, jlink, jlinkstarta, jlinkstartb, jlinkstopa, 
	 jlinkstopb, jsegmenta, jsegmentb, jstarta, jstartb, jstopa, 
	 jstopb;
	float pointstarta[2], pointstartb[2], pointstopa[2], pointstopb[2];


	/*=====================================================================
	 * PURPOSE: To merge contour segments where possible.
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    contouring:  MLINKCLOSED
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:     NextContSeg, GetContPoint, PutContSeg, PutContPoint, 
	 *             PointsEqual, GetContSeg
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900405:  Added action attribute and changed link usage.
	 *    900315:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900315
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Loop until no more segments can be merged. */
	again = TRUE;
L_1000:
	if( again ){

		/* -- Loop on first segment (segment a) from 1 to end. */
		again = FALSE;
		jsegmenta = 0;
L_2000:
		if( nextcontseg( &jsegmenta, &jlevela, &jstarta, &jstopa ) ){
			/* --- Loop on second segment (segment b) from segment a to end. */
			jsegmentb = jsegmenta;
L_3000:
			if( nextcontseg( &jsegmentb, &jlevelb, &jstartb, &jstopb ) ){

				/* -- If segment levels are equal and both segments are not empty: */
				if( (jlevela == jlevelb && jstarta > 0) && jstartb > 
				 0 ){
					/* --- Get start and stop points for both segments. */
					getcontpoint( jstarta, pointstarta, &jlinkstarta, 
					 &jactionstarta );
					getcontpoint( jstopa, pointstopa, &jlinkstopa, 
					 &jactionstopa );
					getcontpoint( jstartb, pointstartb, &jlinkstartb, 
					 &jactionstartb );
					getcontpoint( jstopb, pointstopb, &jlinkstopb, 
					 &jactionstopb );
					/* --- Merge segments a and b into a closed segment if both end points are
					 *     equal or into a larger open segment if either end point is equal. */
					merged = FALSE;
					if( pointsequal( pointstarta, pointstopb ) && 
					 pointsequal( pointstartb, pointstopa ) ){
						jlink = jlinkstartb;
						putcontpoint( jstopa, pointstopa, jlink, jactionstopa );
						jlink = MLINKCLOSED;
						putcontpoint( jstopb, pointstopb, jlink, jactionstopb );
						putcontseg( jsegmenta, jlevela, jstarta, jstopb );
						merged = TRUE;
						}
					else if( pointsequal( pointstarta, pointstopb ) ){
						jlink = jlinkstarta;
						putcontpoint( jstopb, pointstopb, jlink, jactionstopb );
						putcontseg( jsegmenta, jlevela, jstartb, jstopa );
						merged = TRUE;
						}
					else if( pointsequal( pointstartb, pointstopa ) ){
						jlink = jlinkstartb;
						putcontpoint( jstopa, pointstopa, jlink, jactionstopa );
						putcontseg( jsegmenta, jlevela, jstarta, jstopb );
						merged = TRUE;
						}
					/* --- If the segments a and b were merged, delete segment b by setting 
					 *     start and stop to negative numbers.
					 *     Get information on segment a again. This is needed because now the
					 *     segment has at least one new end point. */
					if( merged ){
						putcontseg( jsegmentb, jlevelb, -1, -1 );
						getcontseg( jsegmenta, &jlevela, &jstarta, 
						 &jstopa );
						again = TRUE;
						}
					}
				goto L_3000;
				}
			goto L_2000;
			}
		goto L_1000;
		}

	return;
}

