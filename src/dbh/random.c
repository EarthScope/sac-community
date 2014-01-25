
/** 
 * @file   random.c
 * 
 * @brief  Get a random number
 * 
 */

#include "dbh.h"

/** 
 * Get a random number (DO NOT USE THIS)
 * 
 * @param seed 
 *    Seed to generate the random number
 * 
 * @return 
 *    Random number returned
 *
 * @bug Users should use the internal random number generator
 *
 */
double 
dbh_random(int *seed) {
        double random_v;

	*seed = 2045**seed + 1;
	*seed = *seed - (*seed/1048576)*1048576;
	random_v = (double)( *seed + 1 )/1048577.0;

	return( random_v );
}

