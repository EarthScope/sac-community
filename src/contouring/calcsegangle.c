/** 
 * @file   calcsegangle.c
 *
 * @brief  Calculate angle between two points
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"

#define PI M_PI

void 
calcsegangle(point1, point2, angle)
float point1[], point2[], *angle;
{

	float *const Point1 = &point1[0] - 1;
	float *const Point2 = &point2[0] - 1;


	/*=====================================================================
	 * PURPOSE: Calculate angle (slope) between two points on a segment.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    point1:  First data point. [ra=2]
	 *    point2:  Second data point. [ra=2]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    angle:   Angle in radians. Angle is from horizontal and 
	 *             constrained to be between -pi/2 and +pi/2. [r]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:        PI
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900418:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900418
	 *===================================================================== */
	/* - Compute angle. */
        if ((Point2[2] == Point1[2]) && (Point2[1] == Point1[1])) *angle = 0.0;
        else
	  *angle = atan2( Point2[2] - Point1[2], Point2[1] - Point1[1] );

	/* - Constrain angle to be between -pi/2 and +pi/2. */

	if( *angle > 0.5*PI ){
		*angle = *angle - PI;
                if ( fabs(*angle) < RNDOFF ) *angle = 0.0;
		}
	else if( *angle < -0.5*PI ){
		*angle = *angle + PI;
                if ( fabs(*angle) < RNDOFF ) *angle = 0.0;
		}
	return;
}

