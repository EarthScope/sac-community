/** 
 * @file   xcrrcp.c
 * 
 * @brief  Parse a real range-checked token pair
 * 
 */

#include "cpf.h"

/** 
 * Parse real range-checked token pairs.  The second  must be larger than
 *    the first
 * 
 * @param realmn 
 *    Minimum value
 * @param realmx 
 *    Maximum value
 * @param real1 
 *    First value
 * @param real2 
 *    Second value
 * @param nerr 
 *    - 0 on Success
 *
 * @date   820608:  Original version.
 *
 */
void 
xcrrcp(double  realmn, 
       double  realmx, 
       double *real1, 
       double *real2, 
       int    *nerr) {

	*nerr = 0;

	/* - Loop on each token in command: */
L_1000:
	if( lcmore( nerr ) ){

		if( lcrrcp( realmn, realmx, real1, real2 ) ){
		}
		else{
			/* -- Bad syntax. */
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();
		}
		goto L_1000;
	}

	return;
}

