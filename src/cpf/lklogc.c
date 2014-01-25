/** 
 * @file   lklogc.c
 * 
 * @brief  Parse a keyed logical/character value
 * 
 */

#include "cpf.h"
#include "bool.h"

/** 
 * Parse a keyed logical/character value command construct
 * 
 * @param kkey 
 *    Keyword to search for
 * @param kkey_s 
 *    Length of \p kkey
 * @param logv 
 *    Logical value on output
 * @param kchar 
 *    Character string on output
 * @param kchar_s 
 *    Length of \p kchar
 * 
 * @return 
 *    - TRUE if the value was found
 *    - FALSE if the value was not found
 *
 * @date   890925:  Fixed bug involving setting of "logv".
 * @date   860306:  Original version.
 *
 */
int 
lklogc(char *kkey, 
       int   kkey_s, 
       int  *logv, 
       char *kchar, 
       int   kchar_s) {

	int lklogc_v;
	int nchar, nerr, nret;

	/* - Determine length of character variable. */
	nchar = (kchar_s - 1);

	/* - Check for key. */
	lklogc_v = lckey( kkey,kkey_s );

	/* - Get logical variable from next symbol if key was found.
	 * - Perform standard error recovery if not found. */

	if( lklogc_v ){
L_2000:
		if( lclog( logv ) ){
		}
		else if( lcchar( nchar, kchar,kchar_s, &nret ) ){
			*logv = TRUE;
		}
		else{
			cfmt( "Need an \"on\", an \"off\", or an alpha:",38 );
			cresp();
			if( lcmore( &nerr ) )
				goto L_2000;
		}
	}

	return( lklogc_v );
}

