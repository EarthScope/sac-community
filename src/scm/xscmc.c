
#include "scm.h"


#include "msg.h"

void /*FUNCTION*/ xscmc(index, nerr)
int index, *nerr;
{



	/*=====================================================================
	 * PURPOSE: To execute a MOD command given its index number.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    INDEX:   The index number of the command.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 0901.
	 *=====================================================================
	 * MODULE/LEVEL: SCM/1
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  GTOUTM, XRQ, XRTM, XRTR, XRMEAN, XTAPER, XROTATE
	 *             XINTERPOLATE, XQUANTIZE, XSTRETCH, XREVERSE, XDECIMATE
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    870812:  Deleted RIR command.  Function replaced by TRANSFER.
	 *    870209:  Moved DECIMATE command from SAM to SCM.
	 *    870203:  Added SMOOTH command.
	 *    861129:  Added INTERPOLATE, QUANTIZE, STRETCH, and REVERSE.
	 *    821025:  Original version.
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
		case 6: goto L_600;
		case 7: goto L_700;
		case 8: goto L_800;
		case 9: goto L_900;
		case 10: goto L_1000;
		case 11: goto L_1100;
		case 12: goto L_1200;
		case 13: goto L_1300;
                case 14: goto L_1400;
		}

	/* - Error return if bad index value. */

	*nerr = 901;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XSCMC",9 );
	goto L_8888;

	/* - Command 01: RQ */

L_100:
	xrq( nerr );
	goto L_8888;

	/* - Command 02: RIR */

L_200:
	*nerr = 1013;
	setmsg( "ERROR", *nerr );
	apcmsg( "TRANSFER with the LLSN instrument type.",40 );
	goto L_8888;

	/* - Command 03: RGLITCHES */

L_300:
	xrglitches( nerr );
	goto L_8888;

	/* - Command 04: RTR */

L_400:
	xrtr( nerr );
	goto L_8888;

	/* - Command 05: RMEAN */

L_500:
	xrmean( nerr );
	goto L_8888;

	/* - Command 06: TAPER */

L_600:
	xtaper( nerr );
	goto L_8888;

	/* - Command 07: ROTATE */

L_700:
	xrotate( nerr );
	goto L_8888;

	/* - Command 08: INTERPOLATE */

L_800:
	xinterpolate( nerr );
	goto L_8888;

	/* - Command 09: QUANTIZE */

L_900:
	xquantize( nerr );
	goto L_8888;

	/* - Command 10: STRETCH */

L_1000:
	xstretch( nerr );
	goto L_8888;

	/* - Command 11: REVERSE */

L_1100:
	xreverse( nerr );
	goto L_8888;

	/* - Command 12: SMOOTH */
L_1200:
	xsmooth( nerr );
	goto L_8888;

	/* - Command 13: DECIMATE */

L_1300:
	xdecimate( nerr );
	goto L_8888;

L_1400:
        xlinefit( nerr );
        goto L_8888;

L_8888:
	return;

} /* end of function */

