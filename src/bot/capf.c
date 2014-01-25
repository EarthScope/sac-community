/** 
 * @file   capf.c
 * 
 * @brief  Close a Pick file
 * 
 */

#include "eam.h"
#include "bool.h"
#include "co.h"

/** 
 * Close the Alphanumeric pick file (APF) if open
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - Non-Zero on Failure
 *
 * @date   820810:  Documented subroutine.
 * @date   820303:  Only call ZCLOSE if LCIPF is .TRUE.
 *
 */
void 
capf(int *nerr) {

	/* - Close APF if open. */
	if( cmeam.lapfop )
		zcloses( &cmeam.napfun, nerr );

	/* - Set flag showing APF is closed. */

	cmeam.lapfop = FALSE;

	return;

}

