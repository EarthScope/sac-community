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
#include "amf.h"
#include "contouring.h"

void 
newcontpoint(point, link, action, number)
float point[];
int link, action, *number;
{
	float *const Point = &point[0] - 1;
        int *Isacmem;

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
	if( cmcontouring.numpoints < cmcontouring.maxpoints ){
		cmcontouring.numpoints = cmcontouring.numpoints + 1;
		*number = cmcontouring.numpoints;
		*(cmmem.sacmem[cmcontouring.indexpoints] + 2*(*number - 1)) = Point[1];
		*(cmmem.sacmem[cmcontouring.indexpoints] + 2*(*number - 1) + 1) = Point[2];

                Isacmem = (int*)cmmem.sacmem[cmcontouring.indexlinks];
		*(Isacmem + *number - 1) = link;

                Isacmem = (int*)cmmem.sacmem[cmcontouring.indexaction];
		*(Isacmem + *number - 1) = action;
	}
	else{
		fprintf( stdout, "No more room for point storage.\n" );
		exit(0);
	}
	return;
}

