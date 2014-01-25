
#include "uom.h"


#include "msg.h"

void /*FUNCTION*/ xuomc(index, nerr)
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
	 * MODULE/LEVEL: UOM/1
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  GTOUTM, XADD, XSUB, XMUL, XDIV, XSQR, XSQRT, XINT, XABS
	 *             XLOG, XLOG10, XEXP, XEXP10
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
		}

	/* - Error return if bad index value. */

	*nerr = 901;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XUOMC",9 );
	goto L_8888;

	/* - Command 01: ADD */

L_100:
	xadd( nerr );
	goto L_8888;

	/* - Command 02: SUB */

L_200:
	xsub( nerr );
	goto L_8888;

	/* - Command 03: MUL */

L_300:
	xmul( nerr );
	goto L_8888;

	/* - Command 04: DIV */

L_400:
	xdiv( nerr );
	goto L_8888;

	/* - Command 05: SQR */

L_500:
	xsqr( nerr );
	goto L_8888;

	/* - Command 06: SQRT */

L_600:
	xsqrt( nerr );
	goto L_8888;

	/* - Command 07: INT */

L_700:
	xint( nerr );
	goto L_8888;

	/* - Command 08: ABS */

L_800:
	xabs( nerr );
	goto L_8888;

	/* - Command 09:  LOG */

L_900:
	xlog( nerr );
	goto L_8888;

	/* - Command 10:  LOG10 */

L_1000:
	xlog10( nerr );
	goto L_8888;

	/* - Command 11:  EXP */

L_1100:
	xexp( nerr );
	goto L_8888;

	/* - Command 12:  EXP10 */

L_1200:
	xexp10( nerr );
	goto L_8888;

	/* - Command 13: DIF */

L_1300:
	xdif( nerr );
	goto L_8888;

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    831020:  Added LOG, LOG10, EXP, EXP10, and DIF.
	 *    820825:  Original version.
	 *===================================================================== */

} /* end of function */

