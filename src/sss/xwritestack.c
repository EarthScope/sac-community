
#include "sss.h"
#include "dfm.h"
#include "hdr.h"
#include "amf.h"


#include "msg.h"
#include "ssi.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xwritestack(nerr)
int *nerr;
{
	int notused;
	float unused;



	/*=====================================================================
	 * PURPOSE:  To execute the action command WRITESTACK.
	 *           This command writes the summed stack to disk.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag.
	 *=====================================================================
	 * MODULE/LEVEL:  sss/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    MCPFN
	 *    hdr:     itime
	 *    mem:     sacmem
	 *    sss:     nlnsum, ndxsum
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    hdr:     iftype, npts, delta, b, e, depmin, depmax, depmen
	 *    sss:     knmsum
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, cfmt, cresp, lcchar, setmsg, newhdr, wsac0
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    881122:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  881122
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

                /* -- "COMMIT|RECALLTRACE|ROLLBACK":
                      how to treat existing data */
                if ( lckeyExact ( "COMMIT" , 7 ) )
                        cmdfm.icomORroll = COMMIT ;
                else if (lckeyExact ( "RECALLTRACE" , 12 ) )
                        cmdfm.icomORroll = RECALL ;
                else if ( lckeyExact ( "RECALL" , 7 ) )
                        cmdfm.icomORroll = RECALL ;
                else if ( lckeyExact ( "ROLLBACK" , 9 ) )
                        cmdfm.icomORroll = ROLLBACK ;

		/* -- "filename":  define new filename for write. */
		else if( lcchar( MCPFN, kmsss.knmsum,MCPFN+1, &notused ) )
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

	/* - Make sure a sum has been calculated. */

	if( cmsss.ndxsum <= 0 ){
		*nerr = 5113;
		setmsg( "ERROR", *nerr );
		goto L_8888;
	}

	/* EXECUTION PHASE: */

        /* - Commit or rollback data according to lmore and cmdfm.icomORroll */
        alignFiles ( nerr ) ;
	if ( *nerr )
	    return ;


	/* - Set up the header variables for the sum. */

	newhdr();

	*iftype = *itime;
	*delta = cmsss.del;
	*npts = cmsss.nlnsum;
	*b = 0.;
	*e = *b + *delta*(float)( *npts - 1 );

	extrma( cmmem.sacmem[cmsss.ndxsum], 1, *npts, depmin, depmax, depmen );

	/* - Write sum to disk. */

	wsac0( kmsss.knmsum, &unused, cmmem.sacmem[cmsss.ndxsum], nerr, MCPFN+1 );

L_8888:
	return;

} /* end of function */

