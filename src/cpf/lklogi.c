/** 
 * @file   lklogi.c
 * 
 * @brief  Parse a keyed logical/integer value
 * 
 */

#include <string.h>

#include "cpf.h"
#include "bool.h"
#include "com.h"


#include "bot.h"

/** 
 * Parse a keyed logical/integer value command construct
 * 
 * @param kkey 
 *    Keyword to search for
 * @param kkey_s 
 *    Length of \p kkey
 * @param logv 
 *    Logical value on output
 * @param intv 
 *    Integer value on output
 * 
 * @return 
 *    - TRUE if value was found
 *    - FALSE if value was not found
 *
 * @date   870929:  Changes to allow both upper and lower case tokens.
 * @date   820721:  Original version.
 *
 */
int
lklogi(char *kkey, 
       int   kkey_s, 
       int  *logv, 
       int  *intv) {

	int lklogi_v;
	int nerr;

	/* - Check for key. */
	lklogi_v = lckey( kkey,kkey_s );

	/* - Get logical variable from next symbol if key was found.
	 * - Perform standard error recovery if not found. */
	if( lklogi_v ){
L_2000:
    if(lclog(logv)) {}
    else if(lcint(intv)) {
      *logv = TRUE;
    }
		else{
			cfmt( "NEED ON, OFF, OR INTEGER:",26 );
			cresp();
			if( lcmore( &nerr ) )
				goto L_2000;
		}
	}

	return( lklogi_v );
}

