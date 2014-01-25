/** 
 * @file   lkrtw.c
 * 
 * @brief  Parse a keyed reference time window
 * 
 */

#include "cpf.h"
#include "bool.h"

/** 
 * Parse a keyed reference time window command construct
 * 
 * @param kkey 
 *    Keyword to search for
 * @param kkey_s 
 *    Length of \p kkey
 * @param lrtw 
 *    - TRUE if reference time window was turned on
 *    - FALSE if reference time window was turned off
 *    - Otherwise not changed
 * @param krtw 
 *    Reference time window
 *    First the the starting time, second is the stopping time
 * @param krtw_s 
 *    Length of \p krtw
 * @param ortw 
 *    Reference time window offset times 
 *    First is the starting offset, second is the stopping offset
 * 
 * @return 
 *    - TRUE if the reference time window was found
 *    - FALSE if the reference time window was not found
 *
 * @date   820623:  Changed form of output arguments.
 * @date   820610:  Rewrote to use standard parsing functions.
 * @date   820312:  Factored test for key to LCKEY function.
 * @date   810206:  Original version.
 *
 */
int
lkrtw(char  *kkey, 
      int    kkey_s, 
      int   *lrtw, 
      char  *krtw, 
      int    krtw_s, 
      double *ortw) {

	int nerr;

	/* - Check for key. */
	if(!lckey( kkey,kkey_s )) {
    return FALSE;
  }

	/* - Use LCRTW to perform parsing.
	 * - Perform standard error recovery if not found. */
L_2000:
	if( lcrtw( lrtw, krtw,krtw_s, ortw ) ){ 
  } else {
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
		if( lcmore( &nerr ) )
		  goto L_2000;
	}
  
  return TRUE;
}

