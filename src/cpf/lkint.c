/** 
 * @file   lkint.c
 * 
 * @brief  Parse a keyed integer 
 * 
 */

#include "cpf.h"
#include "com.h"
#include "bool.h"

/** 
 * Parse a keyed integer variable command construct
 * 
 * @param kkey 
 *    Keyword to search for
 *      Append a dollar sign ("$") to end of keyword.
 *      Append a pound sign ("#") to end of minimum allowed
 *      abbreviation if it is more than a single character.
 * @param kkey_s 
 *    Length of \p kkey
 * @param intv 
 *    Integer variable on output
 * 
 * @return 
 *    - TRUE if the integer was found
 *    - FALSE  if the integer was not found
 *
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   820415:  Added information hiding logic.
 * @date   820312:  Factored test for key to LCCKEY function.
 * @date   810207:  Original version.
 *
 */
int 
lkint(char *kkey, 
      int   kkey_s, 
      int  *intv) {

	/* - Check for key. */
	if(!lckey( kkey,kkey_s )) {
    return FALSE;
  }

  lcint(intv);
  return TRUE;
}

