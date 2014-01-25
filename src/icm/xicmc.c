
#include "icm.h"


#include "msg.h"

void /*FUNCTION*/ xicmc(index, nerr)
int index, *nerr;
{



	/*=====================================================================
	 * PURPOSE: To execute a Instrument Correction Module (ICM) command 
	 *          given its index number.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    INDEX:   The index number of the command.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 0901.
	 *=====================================================================
	 * MODULE/LEVEL: ICM/1
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    ICM:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  GTOUTM, XTRANSFER
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    870316:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870316
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Jump to correct command based upon its index number. */

	switch( index ){
		case 1: goto L_100;
		case 2: goto L_200;
		}

	/* - Error return if bad index value. */

	*nerr = 901;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XICMC",9 );
	goto L_8888;


L_100:
	/* - Command 01: TRANSFER */
	xtransfer( nerr );
	goto L_8888;

L_200:
	/* - Command 02: WHITEN */
	xprewit( nerr ) ;
	goto L_8888 ;

L_8888:
	return;

} /* end of function */

