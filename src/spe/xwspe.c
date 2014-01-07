
#include "spe.h"
#include "hdr.h"
#include "amf.h"


#include "msg.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"

extern sac *spe;
extern float *spespe;
void /*FUNCTION*/ xwspe(nerr)
int *nerr;
{
	int notused;
	float unused;

	/*=====================================================================
	 * PURPOSE:  To execute the action command WRITESPE.
	 *           This command writes the spectral estimate to disk.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag.
	 *=====================================================================
	 * MODULE/LEVEL:  spe/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    MCPFN
	 *    hdr:     npts, b, delta, iftype
	 *    spe:     knmspe, lspe, samfrq, nlnspe, ndxspe
	 *    mem:     sacmem
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    hdr:     nsnpts, sb, sdelta, iftype, npts, delta, b, e
	 *    spe:     knmspe
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, cfmt, cresp, lcchar, setmsg, wsac0
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    060627:  Produce error message explicitly after wsac0 failure (grh/ub)
         *    880422:  Writing full header instead of just the minimum. 
	 *    820817:  Changed to newest set of parsing and checking functions.
	 *    810120:  Changed to output message retrieval from disk.
	 *    801219:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  880422
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

		/* -- "filename":  define new filename for write. */
		if( lcchar( MCPFN, kmspe.knmspe,MCPFN+1, &notused ) )
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

	if( !cmspe.lspe ){
	    *nerr = 5004;
	    setmsg( "ERROR", *nerr );
	    goto L_8888;
	}

	/* EXECUTION PHASE: */

	/* - Set up the header variables for the spectral estimate. */

	spe->h->nsnpts = spe->h->npts;
	spe->h->sb = spe->h->b;
	spe->h->sdelta = spe->h->delta;

	spe->h->iftype = ITIME;
	spe->h->delta = cmspe.samfrq/(float)( cmspe.nlnspe - 1 );
	spe->h->npts = cmspe.nlnspe/2 + 1;
	spe->h->b = 0.;
	spe->h->e = spe->h->b + spe->h->delta*(float)( spe->h->npts - 1 );

	extrma( spespe, 1, spe->h->npts, &spe->h->depmin, &spe->h->depmax, &spe->h->depmen );

	/* - Write spectral estimate to disk. */

	wsac0( kmspe.knmspe, &unused, spespe, nerr, MCPFN+1 );
        if( *nerr != 0 ) {
           setmsg( "ERROR", *nerr );
           apcmsg( kmspe.knmspe,MCPFN+1 );
           }

L_8888:
	return;

} /* end of function */

