/** 
 * @file   lklog2.c
 *
 * @brief  Parse a keyed logical valye
 * 
 */

#include <string.h>

#include "cpf.h"
#include "bool.h"
#include "bot.h"


#include "co.h"

/** 
 * Parse a keyed logical variable command construct.
 * 
 * @param kkey 
 *    Keyword to search for
 * @param kkey_s 
 *    Length of \p kkey
 * @param ktrue 
 *    Turns on logical value
 * @param ktrue_s 
 *    Length of \p ktrue
 * @param kfalse 
 *    Turns off logical value
 * @param kfalse_s 
 *    Length of \p kfalse
 * @param logv 
 *    Logical value on output
 * 
 * @return 
 *    - TRUE if logical value was found
 *    - FALSE if logical value was not found
 *
 * @date   961009:  Fixed 2 bugs: 1. endless loop if token was bad
 *                                2. the last letter of the token was ignored
 * @date   820622:  Original version.
 *
 */
int 
lklog2(char *kkey, 
       int   kkey_s, 
       char *ktrue, 
       int   ktrue_s, 
       char *kfalse, 
       int   kfalse_s, 
       int  *logv) {

	/* - Check for key. */
	if(!lckey( kkey,kkey_s )) {
    return FALSE;
  }
  
  lclog2(ktrue, ktrue_s, kfalse, kfalse_s, logv);
  return TRUE;
}

