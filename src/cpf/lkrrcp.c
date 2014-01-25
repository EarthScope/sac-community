/** 
 * @file   lkrrcp.c
 * 
 * @brief  Parse a keyed range checked real pair
 * 
 */

#include <stdio.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"

/** 
 * Parse a keyed range checked real pair variable command construct
 * 
 * @param kkey 
 *    Keyword to search fo
 * @param kkey_s 
 *    Length of \p kkey
 * @param realmn 
 *    Minimum value
 * @param realmx 
 *    Maximum value
 * @param realv1 
 *    First real value on output
 * @param realv2 
 *    Second real value on output
 * 
 * @return 
 *    - TRUE if pair was found
 *    - FALSE if pair was not found
 *
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   820312:  Factored test for key to LCCKEY function.
 * @date   810207:  Original version.
 *
 */
int
lkrrcp(char   *kkey, 
       int     kkey_s, 
       double  realmn, 
       double  realmx, 
       double *realv1, 
       double *realv2) {

	if(!lckey( kkey,kkey_s )) {
    return FALSE;
  }

  lcrrcp(realmn, realmx, realv1, realv2);
  return TRUE;
}

