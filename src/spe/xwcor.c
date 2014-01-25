
#include "spe.h"
#include "amf.h"


#include "msg.h"
#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xwcor(nerr)
int *nerr;
{
	float delcor;
	double zero = 0.0 ; /* so it can be passed by reference.  maf 970917 */



	/*=====================================================================
	 * PURPOSE:  To execute the action command WCOR.
	 *           This command writes the correlation function to disk.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag.
	 *=====================================================================
	 * MODULE/LEVEL:  SPE/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    SPE:     LCOR, KNMCOR, NDXCOR, NLNCOR
	 *    DFM:     SACMEM
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SPE:     KNMCOR
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LCCHAR, GTOUTM, WSAC1
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){
	    /* -- "filename":  define name of file to write. */
	    if( lcchar( MCPFN, kmspe.knmcor,MCPFN+1, &cmspe.junk ) )
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

	/* - Make sure a spectral estimate has been calculated. */

	if( !cmspe.lcor ){
	    *nerr = 5003;
	    setmsg( "ERROR", *nerr );
	    goto L_8888;
	}

	/* EXECUTION PHASE: */

	/* - Write the correlation function to disk. */

	delcor = 1./cmspe.samfrq;
	wsac1( kmspe.knmcor, cmmem.sacmem[cmspe.ndxcor], &cmspe.nlnfft,  (float *)&zero, &delcor, nerr, MCPFN+1 );
        if( *nerr != 0 ) setmsg( "ERROR", *nerr );

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    060627:  Produce error message explicitly after wsac1 failure (grh/ub)
         *    820817:  Changed to newest set of parsing and checking functions.
	 *    801219:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850109
	 *===================================================================== */

} /* end of function */

