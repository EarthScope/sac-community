/** 
 * @file   xbreak.c
 * 
 * @brief  Parse "BREAK"
 * 
 */

#include "cnd.h"

/** 
 * Parse the action command "BREAK"
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *
 * @note Global Coupling
 *   - ndolevel: Decremented by one.
 *
 * @date   870817:  Original version.
 *
 */
void 
xbreak(int *nerr) {

	*nerr = 0;
	if( cnd.ndolevel > 0 ){
		skipdo( nerr );
	}
	else{
		/* - Raise error condition if not in an do condition. */
		*nerr = 1;
	}

	return;
}

