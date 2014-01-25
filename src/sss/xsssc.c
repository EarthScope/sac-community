
#include "sss.h"


#include "msg.h"

void /*FUNCTION*/ xsssc(index, nerr)
int index, *nerr;
{



	/*=====================================================================
	 * PURPOSE: To execute a Signal Stacking Subprocess (SSS) command.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    index:   The index number of the command. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred. [i]
	 *             Potential error numbers: 0901.
	 *=====================================================================
	 * MODULE/LEVEL: sss/1
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    sss:     ldtalm, dtalm
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  gtoutm, xzerostack, xglobalstack, xaddstack, xdeletestack
	 *             xchangestack, xliststack, xincrementsta, xtimewindow,
	 *             xsss, xplotstack, xdeletacheck, xdistanceaxis, xtimeaxis,
	 *             xclogr, xvelocityrose, xplotrecords, xvelocitymode
	 *             xwritestack
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    881122:  Added WRITESTACK command.
	 *    881117:  Changed names again.
	 *    860421:  Changed names of most SSS subroutines.
	 *    860306:  Changed STACKWINDOW to TIMEWINDOW.  Added DISTANCEWINDOW.
	 *    850812:  Major revision of subprocess.
	 *    820000:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850812
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
		}

	/* - Error return if bad index value. */

	*nerr = 901;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XSSSC",9 );
	goto L_8888;

	/* - Command 01: SSS: initialize Signal Stacking Subprocess. */

L_100:
	xsss( nerr );
	goto L_8888;

	/* - Command 02: ZEROSTACK:  zero or initialize signal stack. */

L_200:
	xzerostack( nerr );
	goto L_8888;

	/* - Command 03: GLOBALSTACK:  define global or default properties. */

L_300:
	xglobalstack( nerr );
	goto L_8888;

	/* - Command 04: ADDSTACK:  add or include a file in the stack. */

L_400:
	xaddstack( nerr );
	goto L_8888;

	/* - Command 05: DELETESTACK:  delete or exclude a file from the stack. */

L_500:
	xdeletestack( nerr );
	goto L_8888;

	/* - Command 06: CHANGESTACK:  change properties of a file in the stack. */

L_600:
	xchangestack( nerr );
	goto L_8888;

	/* - Command 07: LISTSTACK:  list stack entries and their properties. */

L_700:
	xliststack( nerr );
	goto L_8888;

	/* - Command 08: INCREMENTSTACK:  increment stack properties. */

L_800:
	xincrementsta( nerr );
	goto L_8888;

	/* - Command 09: SUMSTACK:  sum the signals in the stack. */
L_900:
	xsumstack( nerr );
	goto L_8888;

	/* - Command 10: TIMEWINDOW:  define the time window for the stack. */

L_1000:
	xtimewindow( nerr );
	goto L_8888;

	/* - Command 11: PLOTSTACK:  plot the signals in the stack. */

L_1100:
	xplotstack( nerr );
	goto L_8888;

	/* - Command 12: DELTACHECK:  set the sampling rate check option. */

L_1200:
	xdeltacheck( nerr );
	goto L_8888;

	/* - Command 13: DISTANCEAXIS:  set record section distance axis options. */

L_1300:
	xdistanceaxis( nerr );
	goto L_8888;

	/* - Command 14: TIMEAXIS:  set record section time axis options. */

L_1400:
	xtimeaxis( nerr );
	goto L_8888;

	/* - Command 15: DISTANCEWINDOW:  define the distance window properties. */

L_1500:
	xdistancewind( nerr );
	goto L_8888;

	/* - Command 16: VELOCITYROSETTE:  set record section velocity rosette options. */

L_1600:
	xvelocityrose( nerr );
	goto L_8888;

	/* - Command 17: PLOTRECORDSECTION:  plot a record section. */

L_1700:
	xplotrecords( nerr );
	goto L_8888;

	/* - Command 18: VELOCITYMODEL:  define stack velocity model properties. */

L_1800:
	xvelocitymode( nerr );
	goto L_8888;

	/* - Command 19: WRITESTACK: write summed stacked to disk. */

L_1900:
	xwritestack( nerr );
	goto L_8888;

	/* - Command 20: QUITSUB: quit (terminate) subprocess. */

L_2000:
	xquitsss( nerr );
	goto L_8888;

	/* - Command 21: TRAVELTIME: Set travel time curves subprocess. */

L_2100:
	xtraveltime( nerr );
	goto L_8888;

	/* - Command 22: PHASE: Set phases to display */

L_2200:
	xphase( nerr );
	goto L_8888;

L_8888:
	return;

} /* end of function */

