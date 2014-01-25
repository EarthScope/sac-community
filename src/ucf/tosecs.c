/** 
 * @file   tosecs.c
 *
 * @brief  Combine seconds and milliseconds
 * 
 */

#include "ucf.h"

/** 
 * Combine seconds and milliseconds
 * 
 * @param nsec 
 *    Seconds
 * @param nmsec 
 *    Milliseconds
 * 
 * @return 
 *    Combined seconds and milliseconds
 *
 * @date   810000:  Original version.
 *
 */
double 
tosecs(int nsec, 
       int nmsec) {

	float tosecs_v;

	tosecs_v = (float)( nsec ) + 0.001*(float)( nmsec );

	return( tosecs_v );

}

