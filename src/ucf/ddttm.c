/** 
 * @file   ddttm.c
 * 
 * @brief  Compute time difference in seconds between two arrays
 * 
 */

#include "ucf.h"

/** 
 * Compute time difference in seconds between two arrays
 * 
 * @param ndttm1 
 *    Time One, length of 6 (year, day of year, hour, minute, second, microsecond)
 * @param ndttm2 
 *    Time Two
 * @param diff
 *    Output difference in seconds 
 * 
 * @bug The output should really be in seconds and microseconds both as integers
 * @bug Handling of leap years is almost correct
 * @bug Lots of constants here
 * @bug If year spans more than a single year, then the calculation is incorrect
 *
 * @date   970908:  ddttm no longer checks that the year and day of year 
 *                  match.  That is now left to the calling function. maf
 * @date   810130:  Original version.
 *
 */
void 
ddttm(int   *ndttm1, 
      int   *ndttm2, 
      float *diff) {

	int nday;

	int *const Ndttm1 = &ndttm1[0] - 1;
	int *const Ndttm2 = &ndttm2[0] - 1;


	if( 4*(Ndttm2[1]/4) == Ndttm2[1] )
		nday = 366;
	else
		nday = 365;

	*diff =     0.001 * (float) ( Ndttm1[ 6 ] - Ndttm2[ 6 ] ) + 
			    (float) ( Ndttm1[ 5 ] - Ndttm2[ 5 ] ) + 
		   60.000 * (float) ( Ndttm1[ 4 ] - Ndttm2[ 4 ] ) + 
		 3600.000 * (float) ( Ndttm1[ 3 ] - Ndttm2[ 3 ] ) + 
		86400.000 * (float) ( Ndttm1[ 2 ] - Ndttm2[ 2 ] ) + 
	 nday * 86400.000 * (float) ( Ndttm1[ 1 ] - Ndttm2[ 1 ] ) ;

	return;
}

