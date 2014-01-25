
#include "sam.h"
#include "dfm.h"
#include "hdr.h"
#include "amf.h"
#include "bool.h"


#include "ucf.h"
#include "dff.h"

void /*FUNCTION*/ xhan(nerr)
int *nerr;
{
	int j, jdfl, ndx1, ndx1l, ndx2, nlen;

        float *Sacmem;

	/*=====================================================================
	 * PURPOSE:  To execute the action command HANNING.
	 *           This command applies a Hanning window to data in memory.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    HDR:     DEPMIN, DEPMAX, DEPMEN
	 *    DFM:     SACMEM()
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  VFLIST, VFEVEN, GETFIL, EXTRMA, PUTFIL
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* CHECKING PHASE: */

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Check to make sure all files are evenly spaced time series files. */

	vfeven( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Get the next file in DFL, moving header to CMHDR. */

		getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Apply Hanning window to dependent data array.
		 *    The two endpoints are defined separately. */

		ndx1l = ndx1 + nlen - 1;
                Sacmem = cmmem.sacmem[ndx1] + 1;
		for( j = ndx1 + 1; j <= (ndx1l - 1); j++ ){
                        *Sacmem = 0.25*(*(Sacmem-1))+0.5**Sacmem+0.25*(*(Sacmem+1));
                        Sacmem++;
			}

                *(cmmem.sacmem[ndx1]) = *(cmmem.sacmem[ndx1]+1);
                *(cmmem.sacmem[ndx1]+nlen-1) = *(cmmem.sacmem[ndx1]+nlen-2);

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
	 *    820809:  Changed to newest set of parsing and checking functions.
	 *    810224:  Original version.
	 *===================================================================== */

} /* end of function */

