
#include "scm.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "dff.h"

void /*FUNCTION*/ xrmean(nerr)
int *nerr;
{
	int j, jdfl, ndx1, ndx2, nlen;

        float *Sacmem;

	/*=====================================================================
	 * PURPOSE: To parse and execute the action command RMEAN.
	 *          This command removes the mean from all data files.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL:  SCM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    HDR:     DEPMEN
	 *    DFM:     NDFL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    HDR:     DEPMIN, DEPMAX, DEPMEN
	 *    MEM:     SACMEM()
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  GTOUTM, GETFIL, PUTFIL
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* CHECKING PHASE: */

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
	    return;

	/* - Check to make sure all files are time series files. */

	vftime( nerr );
	if( *nerr != 0 )
	    return;

	/* EXECUTION PHASE: */

	/* - Peform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
	    /* -- Get the next file in DFL, moving header to CMHDR. */
	    getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
	    if( *nerr != 0 )
        return;

	    /* -- Remove mean from each data point. */
      rmean( cmmem.sacmem[ndx1], *npts, *depmen);

	    /* -- Update any header fields that may have changed. */
	    *depmin = *depmin - *depmen;
	    *depmax = *depmax - *depmen;
	    *depmen = 0.;

	    /* -- Reverse the steps used in getting the next file in DFL. */

	    putfil( jdfl, nerr );
	    if( *nerr != 0 )
		return;
	}

	/* - Calculate and set new dependent variable range values. */

	setrng();

	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    820621:  Changed to newest set of parsing functions.
	 *=====================================================================
	 * DOCUMENTED:  820621
	 *===================================================================== */

} /* end of function */

