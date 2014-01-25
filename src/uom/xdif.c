
#include "uom.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "ucf.h"
#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xdif(nerr)
int *nerr;
{
	int jdfl, ndx1, ndx2, nlen;



	/*=====================================================================
	 * PURPOSE: To parse and execute the action command DIF.
	 *          This command differentiates data in memory.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  UOM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    UOM:     KDIFTP, IDIFTP, NDIFNF
	 *    DFM:     NDFL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    UOM:     MDIFTP, KDIFTP, IDIFTP, NDIFTP
	 *    HDR:     DEPMIN, DEPMAX, DEPMEN
	 *    MEM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CMFT, CRESP, LCLIST, VFLIST, VFEVEN,
	 *             GETFIL, DIF3, DIF5, EXTRMA, PUTFIL
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

	    /* -- "TWO/THREE/FIVE":  select type of differentiator */
	    if( lclist( (char*)kmuom.kdiftp,9, cmuom.ndiftp, &cmuom.idiftp ) )
	    { /* do nothing */ }

	    /* -- Bad syntax. */
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }
	}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
	    goto L_8888;

	/* CHECKING PHASE: */

	/* - Test for a non-null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Make sure each file is an evenly spaced time series file. */

	vfeven( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - EXECUTION PHASE: */

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
	    /* -- Get the next file in DFL, moving header to CMHDR. */

	    getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Call the specific subroutine to work on this file. */

	    if( cmuom.idiftp == 1 ){
		dif2( cmmem.sacmem[ndx1], nlen, *delta, cmmem.sacmem[ndx1] );
		*npts = *npts - 1;
		*begin = *begin + 0.5**delta;
	    }
	    else if( cmuom.idiftp == 2 ){
		dif3( cmmem.sacmem[ndx1], nlen, *delta, cmmem.sacmem[ndx1] );
		*npts = *npts - 2;
		*begin = *begin + *delta;
	    }
	    else{
		dif5( cmmem.sacmem[ndx1], nlen, *delta, cmmem.sacmem[ndx1] );
		*npts = *npts - 2;
		*begin = *begin + *delta;
	    }

	    Nlndta[ jdfl ] = *npts ;	/* maf 970822 */

	    *ennd = *begin + (float)( *npts - 1 )**delta;

	    /* -- Update any header fields that may have changed. */

	    if( *idep == *idisp ){
		*idep = *ivel;
	    }
	    else if( *idep == *ivel ){
		*idep = *iacc;
	    }
	    else{
		*idep = *iunkn;
	    }
	    extrma( cmmem.sacmem[ndx1], 1, *npts, depmin, depmax, depmen );

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
	 *    831020:  Original version (from XSC DIF and DIFF.)
	 *===================================================================== */

} /* end of function */

