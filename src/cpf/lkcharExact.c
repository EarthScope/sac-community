/** 
 * @file   lkcharExact.c
 * 
 * @brief  Parse a keyed character string
 * 
 * 
 */

#include <string.h>

#include "cpf.h"
#include "com.h"
#include "co.h"
#include "bot.h"
#include "bool.h"


#include "ucf.h"

/** 
 * Parse a keyed character string command construct
 * 
 * @param kkey 
 *   Keyword to search for
 *     - Append a dollar sign ("$") to end of keyword.
 *     - Append a pound sign ("#") to end of minimum allowed
 *        abbreviation if it is more than a single character.
 * @param kkey_s 
 *   Length of \p kkey
 * @param mchar 
 *   Maximum Length of \p kkey
 * @param kchar 
 *   Output character string
 * @param kchar_s 
 *   Length of \p kchar
 * @param nchar 
 *   Length of character string on output
 * 
 * @return 
 *   - TRUE if string was found
 *   - FALSE if string was not found
 *
 * @date   970702:  Original version, based on lkchar.  maf
 * @date   830208:  Allowed for a numeric token to be used as alphanumeric.
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   820312:  Factored test for key to LCCKEY function.
 * @date   810928:  Modified due to changes in TOKENS.
 * @date   810416:  Removed option to use underline as substitute for blank.
 * @date   810208:  Original version.
 *
 */
int
lkcharExact(char *kkey, 
	    int   kkey_s, 
	    int   mchar, 
	    char *kchar, 
	    int   kchar_s, 
	    int  *nchar) {

	/* - Check for key. */
	if(!lckeyExact( kkey,kkey_s )) {
    return FALSE;
  }

  lcchar(mchar, kchar, kchar_s, nchar);

  return TRUE;
}

