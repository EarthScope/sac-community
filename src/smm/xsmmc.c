
#include "smm.h"


#include "msg.h"
#include "cpf.h"

void /*FUNCTION*/ xsmmc(index, nerr)
int index, *nerr;
{



	/*=====================================================================
	 * PURPOSE: To execute a Signal Measurement Module (SMM) command 
	 *          given its index number.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    index:   The index number of the command.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 0901.
	 *=====================================================================
	 * MODULE/LEVEL: smm/1
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    smm:    lmtw, kmtw, omtw
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  gtoutm, xmarktimes, xmarkvalue, xcrtw, xptp, xrms
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    890224:  Added RMS command.
	 *    861128:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861128
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Jump to correct command based upon its index number. */

	switch( index ){
		case 1: goto L_100;
		case 2: goto L_200;
		case 3: goto L_300;
		case 4: goto L_400;
		case 5: goto L_500;
		}

	/* - Error return if bad index value. */

	*nerr = 901;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XSMMC",9 );
	goto L_8888;

	/* - Command 01: MTW (measurement time window) */

L_100:
	xcrtw( &cmsmm.lmtw, (char*)kmsmm.kmtw,9, cmsmm.omtw, nerr );
	goto L_8888;

	/* - Command 02: MARKVALUE */

L_200:
	xmarkvalue( nerr );
	goto L_8888;

	/* - Command 03: MARKTIMES */

L_300:
	xmarktimes( nerr );
	goto L_8888;

	/* - Command 04: MARKPTP */

L_400:
	xmarkptp( nerr );
	goto L_8888;

	/* - Command 05: RMS */

L_500:
	xrms( nerr );
	goto L_8888;

L_8888:
	return;

} /* end of function */

