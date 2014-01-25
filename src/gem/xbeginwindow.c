
#include "gem.h"
#include "gdm.h"


#include "cpf.h"

void /*FUNCTION*/ xbeginwindow(nerr)
int *nerr;
{

	/*=====================================================================
	 * PURPOSE:  To execute the action command BEGINWINDOW.
	 *           This command begins plotting to a specific graphics window.
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
	 *    SACLIB:  LCMORE, LCIRC, CFMT, CRESP, BEGINWINDOW
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    870426:  Moved window checking and creation to beginwindow.
	 *    870316:  Added calls to set color table and color when
	 *             a new window is created.
	 *    870127:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870127
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

		/* -- "n":  the graphics window number. */
		if( lcirc( 1, MWINDOWS, &cmgdm.iwindow ) )
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

	/* EXECUTION PHASE: */

	/* - Begin graphics to the requested window. */

	beginwindow( cmgdm.iwindow, nerr );

L_8888:
	return;

} /* end of function */

