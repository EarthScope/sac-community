/** 
 * @file   getcontpoint.c
 *
 * @brief  Get info about a line point
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"
extern struct contour contour;
void 
getcontpoint(number, point, link, action)
int number;
float point[];
int *link, *action;
{
	float *const Point = &point[0] - 1;

	/*=====================================================================
	 * PURPOSE:  To get information about an existing contouring line point.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    number:  The point number. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    point:   The point. First value is x, second is y. [f2]
	 *    link:    The link to the next point in segment. [i]
	 *    action:  Action to perform at new point. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mem:          sacmem, isacmem
	 *    contouring:   numpoints, indexpoints, indexlinks, indexaction
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900405:  Added action attribute storage.
	 *    900315:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900405
	 *===================================================================== */
	/* PROCEDURE: */
	if( number <= cmcontouring.numpoints ){
    Point[1] = contour.points[number-1].pts[0];
    Point[2] = contour.points[number-1].pts[1];
    *action   = contour.points[number-1].action;
    *link     = contour.points[number-1].link;
	}
	else{
		fprintf( stdout, "Illegal point number: %d getcontpoint \n", number );
		exit(0);
	}

	return;

}

