
#include "sam.h"
#include "amf.h"
#include "dfm.h"
#include "hdr.h"
#include "bool.h"


#include "co.h"
#include "msg.h"
#include "ucf.h"
#include "dbh.h"
#include "dff.h"

#define	MINDATALEN	201
#define	MLENSCRATCH	4297

void /*FUNCTION*/ xhilbert(nerr)
int *nerr;
{
	int jdfl, ndxscratch, ndxsignal, nlenmn, nlnsignal, 
	 notused, ntused;



	/*=====================================================================
	 * PURPOSE: To parse and execute the action command HILBERT.
	 *          This command computes the Hilbert transform.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  sam/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    dfm:     ndfl
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    hdr:     depmin, depmax, depmen
	 *    mem:     sacmem
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  vflist, vfeven, getfil, setmsg, apimsg, allamb, firtrn,
	 *             extrma, putfil, relamb
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    MLENSCRATCH:  Size of scratch space needed for transform. [ip]
	 *    ndxscratch:   Index in SACMEM array for scratch space. [i]
	 *    ndxsignal:    Index in SACMEM array for current signal. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    890223:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  890223
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

	/* - Determine minimum signal size.
	 *   Make sure minimum is not too small for fir filter subroutine. */

	nlenmn = MLARGE;
	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
		getfil( jdfl, FALSE, &ntused, &ntused, &ntused, nerr );
		if( *nerr != 0 )
			goto L_8888;
		nlenmn = min( nlenmn, *npts );
		}

	if( nlenmn < MINDATALEN ){
		*nerr = 1613;
		setmsg( "ERROR", *nerr );
		apimsg( MINDATALEN );
		goto L_8888;
		}

	/* - EXECUTION PHASE: */

	/* - Allocate temporary block for scratch space. */

	allamb( &cmmem, MLENSCRATCH, &ndxscratch, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Get next file from the memory manager.
		 *    (Header is moved into common blocks CMHDR and KMHDR.) */
		getfil( jdfl, TRUE, &nlnsignal, &ndxsignal, &notused, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Compute the Hilbert transform in place. */
		firtrn( "HILBERT", cmmem.sacmem[ndxsignal], nlnsignal, cmmem.sacmem[ndxscratch], 
		 cmmem.sacmem[ndxsignal] );

		/* -- Update any header fields that may have changed. */
		extrma( cmmem.sacmem[ndxsignal], 1, nlnsignal, depmin, depmax, 
		 depmen );

		/* -- Return file to memory manager. */
		putfil( jdfl, nerr );
		if( *nerr != 0 )
			goto L_8888;

		}

	/* - Release scratch space. */

	relamb( cmmem.sacmem, ndxscratch, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

} /* end of function */

