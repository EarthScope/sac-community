/** 
 * @file   lkquot.c
 * 
 * @brief  Parse a keyed character string
 * 
 */

#include <string.h>

#include "cpf.h"
#include "com.h"
#include "co.h"
#include "ucf.h"
#include "bool.h"

/** 
 * Parse a keyed character string command construct
 * 
 * @param kkey 
 *    Keyword to search for
 * @param kkey_s 
 *    Length of \p kkey
 * @param mquot 
 *    Maximum Length of \p kquot
 * @param kquot 
 *    Output character string
 * @param kquot_s 
 *    Length of \p kquot
 * @param nquot 
 *    Length of \p kquot on output
 * 
 * @return 
 *    - TRUE if character string was found
 *    - FALSE if character string was not found
 *
 * @date   910113:  Adjustment of counter jcom is added a la lcquot
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   820312:  Factored test for key to LCCKEY function.
 * @date   810928:  Modified due to changes in TOKENS.
 * @date   810416:  Removed option to use underline as substitute for blank.
 * @date   810208:  Original version.
 *
 */
int 
lkquot(char *kkey, 
       int   kkey_s, 
       int   mquot, 
       char *kquot, 
       int   kquot_s, 
       int  *nquot) {

	if(!lckey( kkey,kkey_s )) {
    return FALSE;
  }

  lcquot(mquot, kquot, kquot_s, nquot);
  return TRUE;
}
