/** 
 * @file   xclog.c
 * 
 * @brief  Parse a simple on/off command
 * 
 */

#include "cpf.h"

/** 
 * Parse a simple on/off type command
 * 
 * @param log 
 *    On output
 *    - TRUE if "on" token was found
 *    - FALSE if "off" token was found
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   820505:  Original version.
 *
 */
void 
xclog(int *log, 
      int *nerr) {

	*nerr = 0;

L_1000:
	if( lcmore( nerr ) ){
		/* -- Turn logical flag on/off. */
		if( lclog( log ) ){
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

