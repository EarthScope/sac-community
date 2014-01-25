/** 
 * @file   warp.c
 * 
 * @brief  Compensate for Frequency Warping
 * 
 */

#include <math.h>

#include "dbh.h"

/** 
 * Applies tangent frequency warping to compensate
 *   for bilinear analog -> digital transformation
 * 
 * @param f 
 *   Original Design Frequency Specification (Hz)
 * @param ts 
 *   Sampling Internal (seconds)
 * 
 * @return 
 *
 * @date 200990  Last Modified:  September 20, 1990
 *
 * @opyright   1990  Regents of the University of California
 *
 * @author   Dave Harris
 *           Lawrence Livermore National Laboratory
 *           L-205
 *           P.O. Box 808
 *           Livermore, CA  94550
 *           USA
 *           (415) 423-0617
 *
 */
double 
warp(double f, 
     double ts)
{
	float angle, twopi, warp_v;


	twopi = 2.0 * M_PI;
	angle = twopi*f*ts/2.;
	warp_v = 2.*tan( angle )/ts;
	warp_v = warp_v/twopi;

	return( warp_v );
}

