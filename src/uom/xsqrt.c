
#include <math.h>

#include "uom.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"
#include "co.h"


#include "ucf.h"
#include "dff.h"

void /*FUNCTION*/ xsqrt(nerr)
int *nerr;
{
	int j, jdfl, ndx1, ndx2, nlen;

	float *Sacmem;

	/*=====================================================================
	 * PURPOSE:  To execute the action command SQRT.
	 *           This command takes the square root of data in memory.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag.
	 *=====================================================================
	 * MODULE/LEVEL:  UOM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    DFM:     SACMEM()
	 *    HDR:     DEPMIN, DEPMAX, DEPMEN
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  VFLIST, VFTIME, VFRNG, GTOUTM, GETFIL, EXTRMA, PUTFIL
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* CHECKING PHASE: */

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Check to make sure all files are time series files. */

	vftime( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Check to make there are no negative numbers in data. */

	vfrng( 0., VLARGE, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Get the next file in DFL, moving header to CMHDR. */

		getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Take square root of each value in the dependent array. */
		Sacmem = cmmem.sacmem[ndx1];
		for( j = ndx1; j <= (ndx1 + nlen - 1); j++ ){
                        *Sacmem = sqrt(*Sacmem);
                        Sacmem++;
			}

		/* -- Update any header fields that may have changed. */

		extrma( cmmem.sacmem[ndx1], 1, nlen, depmin, depmax, depmen );

		/* -- Reverse the steps used in getting the next file in DFL. */

		putfil( jdfl, nerr );
		if( *nerr != 0 )
			goto L_8888;

		}

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    820817:  Changed to newest set of parsing and checking functions.
	 *    820202:  Original version.
	 *===================================================================== */

} /* end of function */

