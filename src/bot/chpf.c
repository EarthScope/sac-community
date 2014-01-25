/** 
 * @file   chpf.c
 * 
 * @brief  Close a HYPO file
 * 
 */

#include <stdio.h>

#include "bool.h"
#include "eam.h"
#include "co.h"

/** 
 * Close the HYPO pick file (HPF) if open
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - Non-Zero on Failure
 *
 * @date   920505:  Added hypo input file end-of-file identifier string.
 * @date   820810:  Documented subroutine.
 * @date   820303:  Only call ZCLOSE if LHPFOP is .TRUE.
 *
 */
void 
chpf(int *nerr) {

	*nerr = 0;

	/* - Close HYPO pick file if open. Write hypo eof string first. */

	if( cmeam.lhpfop ){
                fprintf(cmeam.nhpfun,"%19s\n","10");
		zcloses( &cmeam.nhpfun, nerr );
	}

	/* - Set flag to show that HPF is closed. */

	cmeam.lhpfop = FALSE;

	return;
}

