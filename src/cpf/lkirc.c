/** 
 * @file   lkirc.c
 * 
 * @brief  Parse a keyed range checked integer value
 * 
 */

#include <stdio.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"

/** 
 * Parse a keyed range checked integer variable command construct
 * 
 * @param kkey 
 *    Keyword to search for
 * @param kkey_s 
 *    Length of \p kkey
 * @param intmn 
 *    Minimum value 
 * @param intmx 
 *    Maximum value
 * @param intv 
 *    Intger value on retrun
 * 
 * @return 
 *    - TRUE if integer was found
 *    - FALSE if integer was not found
 *
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   820312:  Factored test for key to LCCKEY function.
 * @date   810207:  Original version.
 *
 */
int
lkirc(char *kkey, 
      int   kkey_s, 
      int   intmn, 
      int   intmx, 
      int  *intv) {

	/* - Check for key. */
	if(!lckey( kkey,kkey_s )) {
    return FALSE;
  }

  lcirc(intmn, intmx, intv);
  return TRUE;
}

