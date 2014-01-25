/** 
 * @file   pointsequal.c
 * 
 * @brief  Compare two points
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"
#include "bool.h"

int 
pointsequal(point1, point2)
float point1[], point2[];
{
	int pointsequal_v;

	float *const Point1 = &point1[0] - 1;
	float *const Point2 = &point2[0] - 1;


	/*=====================================================================
	 * PURPOSE:  To compare two points to see if they are equal.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    point1:  First point. First value is x, second is y.[f2]
	 *    point2:  Second point. [f2]
	 *=====================================================================
	 * FUNCTION VALUE:
	 *    PointsEqual:  .TRUE. if points are equal (the same.)
	 *                  .FALSE. if not.
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    RNDOFF
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900315:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900315
	 *===================================================================== */
	/* PROCEDURE: */
	if( fabs( (Point1[1] - Point2[1])/0.5*(Point1[1] + Point2[1]) ) > 
	 RNDOFF ){
		pointsequal_v = FALSE;
		}
	else if( fabs( (Point1[2] - Point2[2])/0.5*(Point1[2] + Point2[2]) ) > 
	 RNDOFF ){
		pointsequal_v = FALSE;
		}
	else{
		pointsequal_v = TRUE;
		}

	return( pointsequal_v );
}

