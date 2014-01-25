
#include "gem.h"
#include "gdm.h"
#include "bool.h"


#include "msg.h"
#include "bot.h"
#include "cpf.h"

void /*FUNCTION*/ xendframe(nerr)
int *nerr;
{
	int notused ;

	/*=====================================================================
	 * PURPOSE:  To execute the action command ENDFRAME.
	 *           Ends multiple plots to a single graphics frame.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL: GDM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GDM:     MWINDOWS
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, BEGINFRAME
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    981124:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED: 
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */


	if ( lcmore( nerr ) ){

	    /* -- "PRINT":  print the final product. */
	    if( lckey( "PRINT#$", 8 ) ) {
		if ( Lgdon[2] ) {
		    cmgem.lprint = TRUE ;
		    lcchar ( MAXPRNTRNAMELEN   , kmgem.kptrName ,
			     MAXPRNTRNAMELEN+1 , &notused ) ;
		    terminate ( kmgem.kptrName ) ;
		}
		else {
		    setmsg ( "WARNING" , 2402 ) ;
		    outmsg () ;
		    clrmsg () ;
		}
	    }

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

	/* EXECUTION PHASE: */

	/* - Begin graphics to the requested window. */

	endframe( FALSE , nerr );

	if ( *nerr != 0 )
	    goto L_8888;

	cmgem.lframe = TRUE;

L_8888:
	return;

} /* end of function */

