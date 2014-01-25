/** 
 * @file   idttm.c
 * 
 * @brief  Increment Time
 * 
 */

#include "ucf.h"

/** 
 * Increment time
 * 
 * @param ndttm1 
 *    Time one
 * @param secs 
 *    Number of seconds
 * @param ndttm2 
 *    Time two
 *
 * @see inctim
 * @see incdat
 *
 * @bug Should use seconds and micro seconds as integers, not floating point numbers
 * @bug Lots of constants
 *
 * @date   810000:  Original version.
 *
 */
void 
idttm(int    *ndttm1, 
      double  secs, 
      int    *ndttm2) {

	int nexday;

	int *const Ndttm1 = &ndttm1[0] - 1;
	int *const Ndttm2 = &ndttm2[0] - 1;


	inctim( Ndttm1[3], Ndttm1[4], Ndttm1[5], Ndttm1[6], secs, &Ndttm2[3], 
	 &Ndttm2[4], &Ndttm2[5], &Ndttm2[6], &nexday );
	incdat( Ndttm1[1], Ndttm1[2], nexday, &Ndttm2[1], &Ndttm2[2] );

	return;
}

