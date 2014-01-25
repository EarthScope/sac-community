
#include "scm.h"
#include "dfm.h"
#include "amf.h"
#include "bool.h"


#include "dff.h"

void /*FUNCTION*/ xreverse(nerr)
int *nerr;
{
	int jdfl, ndxx, ndxy, nlen;
	float temp;

        float *Sacmem1, *Sacmem2;


	/*=====================================================================
	 * PURPOSE: To parse and execute the action command REVERSE.
	 *          This command reverses the data elements in each file.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:    MCPFN, MCMSG, KBASDR, KSUBDL, KDIRDL, KTYPEA
	 *    DFM:     NDFL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    HDR:     DELTA, NPTS, E, DEPMIN, DEPMAX, DEPMEN
	 *    DFM:     NLNFIL
	 *    MEM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LCIRC, VFLIST, VFEVEN,
	 *             CRNAME, RFIR, GETFIL, EXTRMA, PUTFIL
	 *    DBH:     RSMP, OVLPSV
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* CHECKING PHASE: */

	/* - Test for a non-null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Make sure each file is an evenly spaced time series file. */

	vfeven( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Get next file from the memory manager.
		 *    (Header is moved into common blocks CMHDR and KMHDR.) */
		getfil( jdfl, TRUE, &nlen, &ndxy, &ndxx, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Reverse data points for this data file. */
		Sacmem1 = cmmem.sacmem[ndxy];
                Sacmem2 = cmmem.sacmem[ndxy]+nlen-1;
L_4000:
		if( Sacmem2 > Sacmem1 ){
			temp = *Sacmem1;
			*(Sacmem1++) = *Sacmem2;
			*(Sacmem2--) = temp;
			goto L_4000;
			}

		/* -- Return file to memory manager. */
		putfil( jdfl, nerr );
		if( *nerr != 0 )
			goto L_8888;

		}

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    861201:  Original version based upon an XSC by Dave Harris.
	 *===================================================================== */

} /* end of function */

