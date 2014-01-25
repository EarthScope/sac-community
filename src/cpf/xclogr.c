/** 
 * @file   xclogr.c
 * 
 * @brief  Parse a on/off/real command
 * 
 */

#include "cpf.h"
#include "bool.h"

/** 
 * Parse a simple on/off/real type command
 * 
 * @param log 
 *    - TRUE if "on" token was found
 *    - FALSE if "off" token was found
 *    - TRUE if real token was found
 * @param real 
 *    Set to value of real token if found
 * @param nerr 
 *    - 0 on Success
 *    
 * @date   820505:  Original version.
 *
 */
void 
xclogr(int    *log, 
       double *real, 
       int    *nerr) {

	*nerr = 0;

	/* - Loop on each token in command: */
L_1000:
	if( lcmore( nerr ) ){

		/* -- Turn logical flag on/off. */
		if( lclog( log ) ){
		}
		else if( lcreal( real ) ){
			/* -- Change value of real variable and turn flag on. */
			*log = TRUE;
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

