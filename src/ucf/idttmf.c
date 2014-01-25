/** 
 * @file   idttmf.c
 * 
 * @brief  Time Difference
 * 
 */

#include "ucf.h"

/** 
 * Time difference
 * 
 * @param ndttm1 
 *    Time one
 * @param nsec 
 *    Number of seconds
 * @param nmsec 
 *    Number of milliseconds
 * @param ndttm2
 *    Time two 
 * 
 * @see inctimf
 * @see incdat
 *
 * @bug Seconds and Milliseconds and times should all be a 64 bit integer 
 *       (long long int)
 *
 * @date   810000:  Original version.
 *
 */
void 
idttmf(int  *ndttm1, 
       int   nsec, 
       int   nmsec, 
       int  *ndttm2) {

	int nexday;

	int *const Ndttm1 = &ndttm1[0] - 1;
	int *const Ndttm2 = &ndttm2[0] - 1;

	inctimf( Ndttm1[3], Ndttm1[4], Ndttm1[5], Ndttm1[6], nsec, nmsec, &Ndttm2[3], 
	 &Ndttm2[4], &Ndttm2[5], &Ndttm2[6], &nexday );
	incdat( Ndttm1[1], Ndttm1[2], nexday, &Ndttm2[1], &Ndttm2[2] );

	return;
}

