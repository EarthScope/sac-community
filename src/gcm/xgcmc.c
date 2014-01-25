#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gcm.h"


#include "msg.h"
#include "gdm.h"

void /*FUNCTION*/ xgcmc(index, nerr)
int index, *nerr;
{

	/*=====================================================================
	 * PURPOSE: To execute a graphics control module command given 
	 *          its index number.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    INDEX:   The index number of the command.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 0901.
	 *=====================================================================
	 * MODULE/LEVEL: GCM/1
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * SUBROUTINES CALLED:
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
	apcmsg( "in XGCMC",9 );
	goto L_8888;

	/* - Command 01: BEGINDEVICES --- Begin plotting to one or more graphic devices. */

L_100:
	xbegindevices( nerr );
	goto L_8888;

	/* - Command 02: ENDDEVICES --- End plotting to one or more graphic devices. */

L_200:
	xenddevices( nerr );
	goto L_8888;

	/* - Command 03: ERASE --- Erase screen on graphics terminal. */

L_300:
	erase( nerr );
	goto L_8888;

	/* - Command 04: VSPACE --- Set graphics viewspace. */

L_400:
	xvspac( nerr );
	goto L_8888;

	/* - Command 05: SGF --- Set SAC Graphics File attributes. */

L_500:
	xsgf( nerr );
	goto L_8888;

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    870513:  Deleted INIG, ONG and OFFG.
	 *             Changed name from OFM to GCM.
	 *    841031:  Added SGF command.
	 *    831107:  Moved installation dependant commands to XIDMC.
	 *    830506:  Added FR80 command.
	 *    821007:  Added VSPACE command.
	 *    820824:  Original version.
	 *===================================================================== */

} /* end of function */

