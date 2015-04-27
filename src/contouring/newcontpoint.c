/** 
 * @file   newcontpoint.c
 * 
 * @brief  Put info about a point
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"
extern struct contour contour;

void
newcontpoint(point, link, action, number)
     float point[];
     int link, action, *number;
{
    float *const Point = &point[0] - 1;

        /*=====================================================================
	 * PURPOSE:  To put information about a new contouring line point.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    point:   The point. First value is x, second is y. [f2]
	 *    link:    The link to the next point in segment. [i]
	 *    action:  Action to perform at new point. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    number:  The point number. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    contouring:   maxpoints
	 *=====================================================================
	 * GLOBAL OUTPUT:
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
    if (cmcontouring.numpoints < cmcontouring.maxpoints) {
        cmcontouring.numpoints = cmcontouring.numpoints + 1;
        *number = cmcontouring.numpoints;
        contour.points[*number - 1].pts[0] = Point[1];
        contour.points[*number - 1].pts[1] = Point[2];
        contour.points[*number - 1].link = link;
        contour.points[*number - 1].action = action;
    } else {
        fprintf(stdout, "No more room for point storage.\n");
        exit(0);
    }
    return;
}
