/** 
 * @file   lkreal.c
 * 
 * @brief  Parse a keyed real variable 
 * 
 */
#include <stdio.h>
#include "cpf.h"
#include "com.h"
#include "bool.h"

/** 
 * Parse a keyed real variable command construct
 * 
 * @param kkey 
 *    Keyword to search for
 * @param kkey_s 
 *    Length of \p kkey
 * @param realv 
 *    Real value on output
 * 
 * @return 
 *    - TRUE if real value was found
 *    - FALSE if real value was not found
 *
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   820312:  Factored test for key to LCCKEY function.
 * @date   810207:  Original version.
 *
 */
int 
lkreal(char   *kkey, 
       int     kkey_s, 
       double *realv) {

	if(!lckey( kkey,kkey_s )) {
    return FALSE;
  }

  lcreal(realv);
  return TRUE;
}

