/** 
 * @file   putcontpoint.c
 * 
 * @brief  Put info about a line point
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"

extern struct contour contour;
CONTOURING_EXTERN

void
putcontpoint(int number, float point[], int link, int action)
{
    float *const Point = &point[0] - 1;

        /*=====================================================================
	 * PURPOSE:  To put information about an existing contouring line point.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    number:  The point number. [i]
	 *    point:   The point. First value is x, second is y. [f2]
	 *    link:    The link to the next point in segment. [i]
	 *    action:  Action to perform at new point. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    contouring:   numpoints, indexpoints, indexlinks
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    mem:          sacmem, isacmem
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900405:  Added action attribute storage.
	 *    900315:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900405
	 *===================================================================== */
    /* PROCEDURE: */
    if (number <= cmcontouring.numpoints) {
        contour.points[number - 1].pts[0] = Point[1];
        contour.points[number - 1].pts[1] = Point[2];
        contour.points[number - 1].link = link;
        contour.points[number - 1].action = action;

    } else {
        fprintf(stdout, "Illegal point number: %d putcontpoint\n", number);
        exit(0);
    }
    return;
}
