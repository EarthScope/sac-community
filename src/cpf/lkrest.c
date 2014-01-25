/** 
 * @file   lkrest.c
 * 
 * @brief  Parse a keyed rest of the command
 * 
 */

#include "cpf.h"
#include "com.h"
#include "co.h"
#include "bool.h"

/** 
 * Parse a keyed rest of the command construct
 * 
 * @param kkey 
 *    Keyword to search for
 * @param kkey_s 
 *    Length of \p kkey
 * @param mchar 
 *    Maximum length of \p kchar
 * @param kchar 
 *    Character string on output
 * @param kchar_s 
 *    Length of \p kchar
 * @param nchar 
 *    Length of \p kchar on output
 * 
 * @return 
 *    - TRUE if the rest of the command was found
 *    - FALSE if the rest of the command was not found
 *
 * @date   820914:  Original version.
 *
 */
int 
lkrest(char *kkey, 
       int   kkey_s, 
       int   mchar, 
       char *kchar, 
       int   kchar_s, 
       int  *nchar) {

	if(!lckey( kkey,kkey_s )) {
    return FALSE;
  }

  lcrest(mchar, kchar, kchar_s, nchar);
  return TRUE;
}

