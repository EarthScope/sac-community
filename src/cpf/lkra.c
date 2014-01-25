/** 
 * @file   lkra.c
 * 
 * @brief  Parse a keyed real array
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"

/** 
 * Parse a keyed real array command construct
 * 
 * @param kkey 
 *    Keyword to search for
 * @param kkey_s 
 *    Length of \p kkey
 * @param nramn 
 *    Minimum number of reals
 * @param nramx 
 *    Maximum number of reals
 * @param ra 
 *    Real array on output
 * @param nra 
 *    Length of \p ra
 * 
 * @return 
 *    - TRUE if the real array was found
 *    - FALSE if the real array was not found
 *
 * @date   820914:  Original version.
 *
 */
int
lkra(char   *kkey, 
     int     kkey_s, 
     int     nramn, 
     int     nramx, 
     double *ra, 
     int    *nra) {

	if(!lckey( kkey,kkey_s )) {
    return FALSE;
  }

  if(!lcra(nramn, nramx, ra, nra)) {
    return FALSE;
  }
  return TRUE;
}
