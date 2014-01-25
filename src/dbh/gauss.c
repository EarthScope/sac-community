/** 
 * @file   gauss.c
 * 
 * @brief  Calculate a random gaussian number distribution
 */

#include <math.h>

#include "dbh.h"

/** 
 * Calculate a randomly distributed gaussian sequence, white noise
 * 
 * @param seed 
 *    Seed for Random Number Generator
 * @param v1 
 *    Output Point 1
 * @param v2 
 *    Output Point 2
 *
 * @return Nothing
 *
 * @date Documented/Reviewed - This need more documentation
 *
 */
void 
gauss(int       *seed, 
      float     *v1, 
      float     *v2)
{
	double x, y, z;

	x = dbh_random( seed );
	y = dbh_random( seed );
	z = sqrt( -2.0e0*log( x ) );

	*v1 = z*cos( 6.2831853*y );
	*v2 = z*sin( 6.2831853*y );

	return;
} 

