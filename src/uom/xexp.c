
#include <math.h>

#include "uom.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "ucf.h"
#include "dff.h"

void /*FUNCTION*/ xexp(nerr)
int *nerr;
{
	int j, jdfl, ndx1, ndx2, nlen;

	float *Sacmem;

	/*=====================================================================
	 * PURPOSE: To parse and execute the action command EXP.
	 *          This command takes the exponential of each data point.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  UOM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    HDR:     DEPMIN, DEPMAX, DEPMEN
	 *    MEM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  VFLIST, VFTIME, GETFIL, EXTRMA, PUTFIL
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* CHECKING PHASE: */

	/* - Test for a non-null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Make sure each file is a time series file. */

	vftime( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */

	/* - For each file in DFL: */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Get next file from the memory manager.
		 *   (Header is moved into common blocks CMHDR and KMHDR.) */

		getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Take the exponential of each data point. */
                Sacmem = cmmem.sacmem[ndx1];
		for( j = ndx1; j <= (ndx1 + nlen - 1); j++ ){
                        *Sacmem = exp(*Sacmem);
                        Sacmem++;
			}

		/* -- Update any header fields that may have changed. */

		extrma( cmmem.sacmem[ndx1], 1, nlen, depmin, depmax, depmen );

		/* -- Return file to memory manager. */

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
	 *    831020:  Original version.
	 *===================================================================== */

} /* end of function */

