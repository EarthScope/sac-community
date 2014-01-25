/** 
 * @file   calclastpoint.c
 * 
 * @brief  Calculate the last point in a segment
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "contouring.h"
#include "co.h"

#define	MPOINTS	2

void 
calclastpoint(jstop, skiplength, jlast)
int jstop;
double skiplength;
int *jlast;
{
	int jaction[MPOINTS], jcur, jlink[MPOINTS], jpoint[MPOINTS], 
	 jprev, jtemp;
	float accumlength, point[MPOINTS][2];

	int *const Jaction = &jaction[0] - 1;
	int *const Jlink = &jlink[0] - 1;
	int *const Jpoint = &jpoint[0] - 1;


	/*=====================================================================
	 * PURPOSE: To calculate the last point in a segment for a label.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    jstop:       Index to stop point of segment. [i]
	 *    skiplength:  Length to skip from end of segment. [r]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    jlast:       Last point in segment to place label. [i]
	 *                 Point that is at least "skiplength" from end.
	 *                 Set to -1 if segment is shorter than skiplength.
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:   GetContPoint, GetContRlink
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900418:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900418
	 *===================================================================== */
	/* PROCEDURE: */
	accumlength = 0.;
	jprev = 1;
	jcur = 2;

	/* - Get stop point. */

	Jpoint[jprev] = jstop;
	getcontpoint( Jpoint[jprev], &point[jprev - 1][0], &Jlink[jprev], 
	 &Jaction[jprev] );

	/* - For each point in segment, traversing backward from stop point. */

L_1000:
	;

	/* -- Get reverse link. */
	getcontrlink( Jpoint[jprev], &Jpoint[jcur] );

	/* -- If there are more points in segment:
	 * --- Get next point.
	 * --- Accumulate segment length.
	 * --- If accumulated length is ge input length, set last point and return.
	 * --- Otherwise, flip pointers and continue. */
	if( Jpoint[jcur] > 0 ){
		getcontpoint( Jpoint[jcur], &point[jcur - 1][0], &Jlink[jcur], 
		 &Jaction[jcur] );
		accumlength = accumlength + sqrt( powi(point[jcur - 1][0] - 
		 point[jprev - 1][0],2) + powi(point[jcur - 1][1] - point[jprev - 1][1],2) );
		if( accumlength >= skiplength ){
			*jlast = Jpoint[jcur];
			}
		else{
			jtemp = jprev;
			jprev = jcur;
			jcur = jtemp;
			goto L_1000;
			}

		/* -- If segment is shorter than requested length, set last point to
		 *    a negative number and return. */
		}
	else{
		*jlast = -1;
		}
	return;
}

