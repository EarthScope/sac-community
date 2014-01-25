
#include "sam.h"


#include "msg.h"

void /*FUNCTION*/ xsamc(index, nerr)
int index, *nerr;
{



	/*=====================================================================
	 * PURPOSE: To execute a MOD command given its index number.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    index:   The index number of the command.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 0901.
	 *=====================================================================
	 * MODULE/LEVEL: sam/1
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  gtoutm, xdft, xidft, xpsp, xwsp, xrsp,
	 *             xlp, xhp, xbp, xbr, xwnr, xfir, xdec, xhan,
	 *             xunwr, xcorrelate, xkhronhite, xbenioff, xdivomega,
	 *             xmulomega, xhilbert
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    890223:  Added HILBERT and ENVELOPE commands.
	 *    870206:  Moved DECIMATE to SCM.
	 *             Added CORRELATE, KHRONHITE, BENIOFF, DIVW, and MULW.
	 *    830108:  Added UNWRAP command.
	 *    820825:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  890223
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
		case 15: goto L_1500;
		case 16: goto L_1600;
		case 17: goto L_1700;
		case 18: goto L_1800;
		case 19: goto L_1900;
		case 20: goto L_2000;
		case 21: goto L_2100;
		case 22: goto L_2200;
                case 23: goto L_2300;
		}

	/* - Error return if bad index value. */

	*nerr = 901;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XSAMC",9 );
	goto L_8888;

	/* - Command 01: DFT */

L_100:
	xdft( nerr );
	goto L_8888;

	/* - Command 02: IDFT */

L_200:
	xidft( nerr );
	goto L_8888;

	/* - Command 03: PSP */

L_300:
	xpsp( nerr );
	goto L_8888;

	/* - Command 04: WSP */

L_400:
	xwsp( nerr );
	goto L_8888;

	/* - Command 05: RSP */

L_500:
	xrsp( nerr );
	goto L_8888;

	/* - Command 06: LP */

L_600:
	xlp( nerr );
	goto L_8888;

	/* - Command 07: HP */

L_700:
	xhp( nerr );
	goto L_8888;

	/* - Command 08: BP */

L_800:
	xbp( nerr );
	goto L_8888;

	/* - Command 09: BR */

L_900:
	xbr( nerr );
	goto L_8888;

	/* - Command 10: WNR */

L_1000:
	xwnr( nerr );
	goto L_8888;

	/* - Command 11: FIR */

L_1100:
	xfir( nerr );
	goto L_8888;

	/* - Command 12: HAN */

L_1200:
	xhan( nerr );
	goto L_8888;

	/* - Command 13: UNWRAP */

L_1300:
	xunwr( nerr );
	goto L_8888;

	/* - Command 14: CORRELATE */

L_1400:
	xcorrelate( nerr );
	goto L_8888;

	/* - Command 15: KHRONHITE */

L_1500:
	xkhronhite( nerr );
	goto L_8888;

	/* - Command 16: BENIOFF */

L_1600:
	xbenioff( nerr );
	goto L_8888;

	/* - Command 17: DIVOMEGA */

L_1700:
	xdivomega( nerr );
	goto L_8888;

	/* - Command 18: MULOMEGA */

L_1800:
	xmulomega( nerr );
	goto L_8888;

	/* - Command 19: HILBERT */

L_1900:
	xhilbert( nerr );
	goto L_8888;

	/* - Command 20: ENVELOPE */

L_2000:
	xenvelope( nerr );
	goto L_8888;

	/* - Command 21: FILTERDESIGN */

L_2100:
	filterdesign( nerr );
	goto L_8888;

	/* - Command 22: KEEPAM */

L_2200:
	xkeepam( nerr );
	goto L_8888;

        /* - Command 23: CONVOLVE */

L_2300:
        xconvolve( nerr );
        goto L_8888;

L_8888:
	return;

} /* end of function */

