/** 
 * @file   lckey.c
 * 
 * @brief  Parse a key
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpf.h"
#include "bool.h"
#include "com.h"
#include "bot.h"
#include "co.h"

#include "msg.h"

#define	MCHECK	136

/** 
 * Parse a key command construct
 * 
 * @param kkey 
 *    Key to search for
 * @param kkey_s 
 *    Length of \p kkey
 * 
 * @return 
 *    - TRUE if the key was found
 *    - FALSE if the key was not found
 *
 * @note  Append a dollar sign ("$") to end of keyword.
 *        Append a pound sign ("#") to end of minimum allowed
 *        abbreviation if it is more than a single character.
 *        For example, if the keyword is BOTTOM and the minimum
 *        abbreviation is BOT, then KKEY would be 'BOT#TOM$'.
 *        If the pound sign was omitted, then B and BO would
 *        also be acceptable abbreviations.
 * @note  You can prepend an apersand ("&") to beginning of
 *        a keyword if NO abbreviation is to be allowed.
 *        This is an obsolete option that has been retained
 *        for compatibility with older codes.  The same effect
 *        can be achieved by appending a pound sign to the
 *        end of the keyword but before the dollar sign.
 *        For example 'ALPHA#$' is the same as "&ALPHA$'.
 *
 * @note Local Variables
 *   - jsym:    Pointer to current command symbol. Differs
 *              from JCOM if current symbol is a quoted string. [i]
 *   - ncsym:   Number of characters in quoted string. [i]
 *   - jicom:   Number of symbols to increment if key
 *              is found at current command symbol.  Differs
 *              from 1 if current symbol is a quoted string. [i]
 *   - mcheck:  The maximum size of a  possible token. [i]
 *              mcheck should be a multiple of 8.
 *   - mchkmul: The smallest multiple used for the length of mcheck. [i]
 *   - NCKEY:   Number of characters in keyword. [i]
 *   - NCABB:   Number of characters in minimum allowed abbreviation. [i]
 *   - KCHECK:  Local variable used to store keyword. [i]
 *   - NCHECK:  Number of characters used when comparing
 *              input key to current command symbol. [i]
 *   - KTOKEN:  Local variable used to store current command token. [k]
 *
 * @date   901002:  Increased size of ktoken (mcheck) to 136.
 *                  Added warning when passing token longer than ktoken 136.
 * @date   900507:  Copied current token to local storage before converting
 *                  to upper case and checking. (VAX/VMS bug fix.)
 * @date   870803:  Increased size of tokens being checked to 32 characters.
 * @date   870730:  Converted current token to upper case before testing.
 * @date   841029:  Added ability to specify minimum allowed abbreviation.
 * @date   840925:  Changed string check length to shorter of key or token.
 * @date   820312:  Original version.
 *
 */
int 
lckey(char *kkey, 
      int   kkey_s) {

        char kcheck[MCHECK + 1];
        char ktoken[MCHECK + 1];
	int lckey_v, lnoabb;
	int ncabb, ncheck, nckey, ncsym;
  Token *t;

	/* - Determine length of input key (i.e., find trailing dollar sign.) */
	nckey = indexc( kkey,kkey_s, '$' );
  memset(ktoken, ' ', 136);
  memset(kcheck, ' ', 136);
	ktoken[ 136 ] = '\0' ;
	kcheck[ 136 ] = '\0' ;

	/* - If trailing dollar sign is missing find 
	 *   last non-blank character. */
	if( nckey <= 0 )
		nckey = indexb( kkey,kkey_s );

	/* - If character length of key is still 0, 
	 *   set function value to .TRUE. and return immediately.  
	 *   Do not increment command pointer. */
	if( nckey == 0 ){
		lckey_v = TRUE;
		goto L_8888;
	}

	/* - Copy key to local variable, 
	 *    deleting special characters if present. */
	lnoabb = kkey[0] == '&';
	ncabb = indexa( kkey,kkey_s, '#', TRUE, TRUE );
	if( lnoabb ){
                fstrncpy(kcheck,136,kkey+1,kkey_s-2);
		nckey = nckey - 1;
	}
	else if( ncabb > 0 ){
                fstrncpy(kcheck,136,kkey,ncabb-1);
                memcpy(kcheck+ncabb-1,kkey+ncabb,kkey_s - (ncabb + 1));
		nckey = nckey - 1;
	}
	else{
                fstrncpy(kcheck,136,kkey,strlen(kkey));
	}
  if(!(t = arg()) || !token_is_string(t)) {
    return FALSE;
  }
	ncsym = strlen(t->str);

	/* - Determine number of characters to check. */
	if( lnoabb ){
		ncheck = max( nckey, ncsym );
	}
	else{
                ncheck = min(nckey,ncsym);
                ncheck = min(ncheck,MCHECK);
		if( ncabb > 0 )
			ncheck = max( ncheck, ncabb - 1 );
	}

	/* - Convert current command token upper case. */
	modcase( TRUE, ktoken, ncheck, ktoken );

  if(strncasecmp(kcheck, t->str, ncheck) == 0) {
    arg_next();
    return TRUE;
  }
  return FALSE;


L_8888:

	return( lckey_v );

}

int
lcequals() {
  Token *t;
  if((t = arg()) && token_is_equals(t)) {
    arg_next();
    return TRUE;
  }
  return FALSE;
}
int
lccomma() {
  Token *t;
  if((t = arg()) && token_is_comma(t)) {
    arg_next();
    return TRUE;
  }
  return FALSE;
}
