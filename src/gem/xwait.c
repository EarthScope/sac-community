
#include "gem.h"
#include "gam.h"
#include "bool.h"


#include "cpf.h"
#include "exm.h"

void /*FUNCTION*/ xwait(nerr)
int *nerr;
{
	int lmode;



	/*=====================================================================
	 * PURPOSE: To parse the parameter-setting command WAIT.
	 *          This command controls user wait responses at the terminal
	 *          for both plot and text output.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL:  gam/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gam:     lwaitr, lwaite
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:  lcmore, cfmt, cresp, lclog, lckey, lklog, settextwait
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900409:  Added TEXT wait options and changed PLOT wait syntax.
	 *    830818:  Moved from GEM to GAM.
	 *    820614:  Original version.
	 *=====================================================================
	 * KNOWN BUGS:
	 * - This command should be moved to EXM and a call to setplotwait
	 *   added when the GEM is rewritten.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900409
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- TEXT ON|OFF: Turn text wait option on or off. */
		if( lklog( "TEXT$",6, &lmode ) ){
			if( lmode ){
				settextwait( "ON" );
				}
			else{
				settextwait( "OFF" );
				}

			/* -- PLOTS ON|OFF|EVERY: Turn plot wait option on or off. */
			}
		else if( lckey( "PLOTS$",7 ) ){

			if( lclog( &cmgam.lwaitr ) ){
				cmgam.lwaite = FALSE;
				}
			else if( lckey( "EVERY$",7 ) ){
				cmgam.lwaitr = TRUE;
				cmgam.lwaite = TRUE;
				}

			/* -- OBSOLETE SYNTAX BELOW. RETAINED FOR BACKWARD COMPATIBILITY:
			 * -- Turn wait option on or off. */
			}
		else if( lclog( &cmgam.lwaitr ) ){
			cmgam.lwaite = FALSE;

			/* -- Turn wait every time option on. */
			}
		else if( lckey( "EVERY$",7 ) ){
			cmgam.lwaitr = TRUE;
			cmgam.lwaite = TRUE;

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

       
	return;

} /* end of function */

