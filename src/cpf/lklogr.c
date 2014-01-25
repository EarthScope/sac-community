/** 
 * @file   lklogr.c
 * 
 * @brief  Parse a keyed logical/real variable
 * 
 */

#include <string.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"


#include "bot.h"

/** 
 * Parse a keyed logical/real variable command construct
 * 
 * @param kkey 
 *    Keyword to search for
 * @param kkey_s 
 *    Length of \p kkey
 * @param logv 
 *    Output logical variable
 * @param realv 
 *    Output real variable
 * 
 * @return 
 *    - TRUE if a value was found
 *    - FALSE if a value was not found
 *
 * @date   870929:  Changes to allow both upper and lower case tokens.
 * @date   820721:  Original version.
 *
 */
int
lklogr(char   *kkey, 
       int     kkey_s, 
       int    *logv, 
       double *realv) {

	int lklogr_v;
	int nerr;

	/* - Check for key. */
	lklogr_v = lckey( kkey,kkey_s );

	/* - Get logical variable from next symbol if key was found.
	 * - Perform standard error recovery if not found. */

	if( lklogr_v ){
L_2000:
    if(lclog(logv)) {}
    else if(lcreal(realv)) {
      *logv = TRUE;
    }
		else{
			cfmt( "NEED AN \"ON\", AN \"OFF\", OR A REAL:",36 );
			cresp();
			if( lcmore( &nerr ) )
				goto L_2000;
			}
		}

	return( lklogr_v );
}

