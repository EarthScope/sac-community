/** 
 * @file   lkia.c
 * 
 * @brief  Parse a keyed integer array
 * 
 */

#include "cpf.h"
#include "com.h"
#include "bool.h"

/** 
 * Parse a keyed integer array command construct
 * 
 * @param kkey 
 *    Keyword to search for
 *       Append a dollar sign ("$") to end of keyword.
 *       Append a pound sign ("#") to end of minimum allowed
 *       abbreviation if it is more than a single character.
 * @param kkey_s 
 *    Length of \p kkey
 * @param mnint 
 *    Minimum number of integers
 * @param mxint 
 *    Maximum number of integers
 * @param ia 
 *    Integer array on output
 * @param nia 
 *    Length of \p ia
 * 
 * @return 
 *    - TRUE if int array was found
 *    - FALSE if int array was not found
 *
 * @date   900410:  Fixed bug involving negative integers.
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   820415:  Added information hiding logic.
 * @date   820312:  Factored test for key to LCCKEY function.
 * @date   810207:  Original version.
 *
 */
int
lkia(char *kkey, 
     int   kkey_s, 
     int   mnint, 
     int   mxint, 
     int  *ia, 
     int  *nia) {

	/* - Check for key. */
	if(!lckey( kkey,kkey_s )) {
    return FALSE;
  }
  
  lcia(mnint, mxint, ia, nia);
  
  return TRUE;
}

