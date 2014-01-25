/** 
 * @file   setrng.c
 * 
 * @brief  Calculate and set the dependent variable range
 * 
 */

#include <math.h>

#include "dff.h"
#include "gam.h"
#include "bool.h"
#include "dfm.h"
#include "hdr.h"
#include "co.h"


/** 
 * Calculate and set the dependent variable range for all files in 
 *    the data file list.  Find the maximum value from the depmin
 *    and depmax and save them to cmgam.rngmin and cmgam.rngmax.
 * 
 * @date   820818:  Original version.
 *
 */
void 
setrng() {

	int jdfl, ndx1, ndx2, nerr, nlen;

	/* - Initialize range variables. */
	cmgam.rngmin = VLARGE;
	cmgam.rngmax = -VLARGE;

	/* - For each file in DFL: */
	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Get header from memory manager. */
		getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, &nerr );
		if( nerr != 0 )
			goto L_8888;

		/* -- Adjust range variables. */
		cmgam.rngmin = fmin( cmgam.rngmin, *depmin );
		cmgam.rngmax = fmax( cmgam.rngmax, *depmax );
	}

L_8888:
	return;

}
