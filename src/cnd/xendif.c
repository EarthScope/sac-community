/** 
 * @file   xendif.c
 * 
 * @brief  Parse "ENDIF"
 * 
 */
#include "cnd.h"

/** 
 * Parse the action command "ENDIF"
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *
 * @note Global Variables
 *   - niflevel: Decremented by one.
 *
 * @date   870817:  Original version.
 *
 */
void 
xendif(int *nerr) {

	*nerr = 0;
	if( cnd.niflevel > 0 ){
		cnd.niflevel = cnd.niflevel - 1;
	}
	else{
		/* - Raise error condition if not in an if condition. */
		*nerr = 1;
	}
	return;
}

