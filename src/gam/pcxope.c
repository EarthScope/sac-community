#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "gam.h"
#include "gem.h"
#include "bool.h"


#include "gdm.h"

void pcxope(int iope, int iopei)
{
	char _c0[2];

	/*=====================================================================
	 * PURPOSE:
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *=====================================================================
	 * MODULE/LEVEL:
	 *=====================================================================
	 * GLOBAL INPUT:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861112:  Removed calls to ZPEN.  Made them into NO-OPs.
	 *    830623:  Added enviroment options with arbitrary integer argument.
	 *    820000:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Jump to correct option based upon its index number. */
	switch( iope ){
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
		case 24: goto L_2400;
		case 25: goto L_2500;
		case 26: goto L_2600;
		case 27: goto L_2700;
		case 28: goto L_2800;
		case 29: goto L_2900;
		case 30: goto L_3000;
		case 31: goto L_3100;
		case 32: goto L_3200;
		case 33: goto L_3300;
		case 34: goto L_3400;
		case 35: goto L_3500;
		case 36: goto L_3600;
		case 37: goto L_3700;
		case 38: goto L_3800;
		case 39: goto L_3900;
		case 40: goto L_4000;
		case 41: goto L_4100;
		case 42: goto L_4200;
		case 43: goto L_4300;
		case 44: goto L_4400;
		case 45: goto L_4500;
		case 46: goto L_4600;
		case 47: goto L_4700;
		case 48: goto L_4800;
		case 49: goto L_4900;
		case 50: goto L_5000;
		case 51: goto L_5100;
		case 52: goto L_5100;
		case 53: goto L_5100;
		case 54: goto L_5100;
		case 55: goto L_5100;
		case 56: goto L_5100;
		case 57: goto L_5100;
		case 58: goto L_5100;
		case 59: goto L_5900;
		case 60: goto L_6000;
		case 61: goto L_6100;
		case 62: goto L_6200;
		}

	/* - Option 01: HL --- Set horizontal text justification to left. */

L_100:
	strcpy( kmgem.khjust, "LEFT    " );
	settextjust( kmgem.khjust, kmgem.kvjust );
	goto L_8888;

	/* - Option 02: HC --- Set horizontal text justification to center. */

L_200:
	strcpy( kmgem.khjust, "CENTER  " );
	settextjust( kmgem.khjust, kmgem.kvjust );
	goto L_8888;

	/* - Option 03: HR --- Set horizontal text justification to right. */

L_300:
	strcpy( kmgem.khjust, "RIGHT   " );
	settextjust( kmgem.khjust, kmgem.kvjust );
	goto L_8888;

	/* - Option 04: VB --- Set vertical text justification to bottom. */

L_400:
	strcpy( kmgem.kvjust, "BOTTOM  " );
	settextjust( kmgem.khjust, kmgem.kvjust );
	goto L_8888;

	/* - Option 05: VC --- Set vertical text justification to center. */

L_500:
	strcpy( kmgem.kvjust, "CENTER  " );
	settextjust( kmgem.khjust, kmgem.kvjust );
	goto L_8888;

	/* - Option 06: VT --- Set vertical text justification to top. */

L_600:
	strcpy( kmgem.kvjust, "TOP     " );
	settextjust( kmgem.khjust, kmgem.kvjust );
	goto L_8888;

	/* - Option 07: ST --- Set text size to tiny. */

L_700:
	cmgem.chht = cmgem.txsiz[1-1];
	cmgem.chwid = cmgem.txrat*cmgem.chht;
	settextsize( cmgem.chwid, cmgem.chht );
	goto L_8888;

	/* - Option 08: SS --- Set text size to small. */

L_800:
	cmgem.chht = cmgem.txsiz[2-1];
	cmgem.chwid = cmgem.txrat*cmgem.chht;
	settextsize( cmgem.chwid, cmgem.chht );
	goto L_8888;

	/* - Option 09: SM --- Set text size to medium. */

L_900:
	cmgem.chht = cmgem.txsiz[3-1];
	cmgem.chwid = cmgem.txrat*cmgem.chht;
	settextsize( cmgem.chwid, cmgem.chht );
	goto L_8888;

	/* - Option 10: SL --- Set text size to large. */

L_1000:
	cmgem.chht = cmgem.txsiz[4-1];
	cmgem.chwid = cmgem.txrat*cmgem.chht;
	settextsize( cmgem.chwid, cmgem.chht );
	goto L_8888;

	/* - Option 11: Q1 --- Set text quality to 1 (string-precision). */

L_1100:
	settexttype( "HARDWARE" );
	goto L_8888;

	/* - Option 12: Q2 --- Set text quality to 2 (character-precision). */

L_1200:
	settexttype( "HARDWARE" );
	goto L_8888;

	/* - Option 13: Q3 --- Set text quality to 3 (stroke-precision). */

L_1300:
	settexttype( "SOFTWARE" );
	goto L_8888;

	/* - Option 14: Q4 --- Set text quality to 4 (Hershey characters). */

L_1400:
	settexttype( "SOFTWARE" );
	goto L_8888;

	/* - Option 15: F1 --- Set text font to 1. */

L_1500:
	settextfont( 1 );
	goto L_8888;

	/* - Option 16: F2 --- Set text font to 2. */

L_1600:
	settextfont( 2 );
	goto L_8888;

	/* - Option 17: F3 --- Set text font to 3. */

L_1700:
	settextfont( 3 );
	goto L_8888;

	/* - Option 18: F4 --- Set text font to 4. */

L_1800:
	settextfont( 4 );
	goto L_8888;

	/* - Option 19: F5 --- Set text font to 5. */

L_1900:
	settextfont( 5 );
	goto L_8888;

	/* - Option 20: F6 --- Set text font to 6. */

L_2000:
	settextfont( 6 );
	goto L_8888;

	/* - Option 21: F7 --- Set text font to 7. */

L_2100:
	settextfont( 7 );
	goto L_8888;

	/* - Option 22: F8 --- Set text font to 8. */

L_2200:
	settextfont( 8 );
	goto L_8888;

	/* - Option 23: AT --- Set arrow head size to tiny. */

L_2300:
	cmgam.pcamul = 1.;
	goto L_8888;

	/* - Option 24: AS --- Set arrow head size to small. */

L_2400:
	cmgam.pcamul = 2.;
	goto L_8888;

	/* - Option 25: AM --- Set arrow head size to medium. */

L_2500:
	cmgam.pcamul = 3.;
	goto L_8888;

	/* - Option 26: AL --- Set arrow head size to large. */

L_2600:
	cmgam.pcamul = 4.;
	goto L_8888;

	/* - Option 27: AF --- Set arrow head type to filled. */

L_2700:
	cmgam.lpcafi = TRUE;
	goto L_8888;

	/* - Option 28: AU --- Set arrow head type to unfilled. */

L_2800:
	cmgam.lpcafi = FALSE;
	goto L_8888;

	/* - Option 29: AV --- Set arrow shaft type to visible. */

L_2900:
	cmgam.lpcavi = TRUE;
	goto L_8888;

	/* - Option 30: AI --- Set arrow shaft type to invisible. */

L_3000:
	cmgam.lpcavi = FALSE;
	goto L_8888;

	/* - Option 31: L1 --- Set linestype to 1 (solid). */

L_3100:
	setlinestyle( 1 );
	goto L_8888;

	/* - Option 32: L2 --- Set linestype to 2. */

L_3200:
	setlinestyle( 2 );
	goto L_8888;

	/* - Option 33: L3 --- Set linestype to 3. */

L_3300:
	setlinestyle( 3 );
	goto L_8888;

	/* - Option 34: L4 --- Set linestype to 4. */

L_3400:
	setlinestyle( 4 );
	goto L_8888;

	/* - Option 35: W1 --- Set linewidth to 1 (thinnest). */

L_3500:
	setlinewidth(1);
	goto L_8888;

	/* - Option 36: W2 --- Set linewidth to 2. */

L_3600:
	setlinewidth(2);
	goto L_8888;

	/* - Option 37: W3 --- Set linewidth to 3. */

L_3700:
	setlinewidth(3);
	goto L_8888;

	/* - Option 38: W4 --- Set linewidth to 4. */

L_3800:
	setlinewidth(4);
	goto L_8888;

	/* - Option 39: N2 --- Set number of polygon sides to 2. */

L_3900:
	cmgam.npcpsi = 2;
	goto L_8888;

	/* - Option 40: N3 --- Set number of polygon sides to 3. */

L_4000:
	cmgam.npcpsi = 3;
	goto L_8888;

	/* - Option 41: N4 --- Set number of polygon sides to 4. */

L_4100:
	cmgam.npcpsi = 4;
	goto L_8888;

	/* - Option 42: N5 --- Set number of polygon sides to 5. */

L_4200:
	cmgam.npcpsi = 5;
	goto L_8888;

	/* - Option 43: N6 --- Set number of polygon sides to 6. */

L_4300:
	cmgam.npcpsi = 6;
	goto L_8888;

	/* - Option 44: N7 --- Set number of polygon sides to 7. */

L_4400:
	cmgam.npcpsi = 7;
	goto L_8888;

	/* - Option 45: N8 --- Set number of polygon sides to 8. */

L_4500:
	cmgam.npcpsi = 8;
	goto L_8888;

	/* - Option 46: N9 --- Set number of polygon sides to 9. */

L_4600:
	cmgam.npcpsi = 9;
	goto L_8888;

	/* - Option 47: NO-OP */

L_4700:
	;
	goto L_8888;

	/* - Option 48: NO-OP */

L_4800:
	;
	goto L_8888;

	/* - Option 49: NO-OP */

L_4900:
	;
	goto L_8888;

	/* - Option 50: NO-OP */

L_5000:
	;
	goto L_8888;

	/* - Option 51-58: Cx --- Set color to one of the standard colors. */

L_5100:
        _c0[0] = kmgam.kope[iope - 1][1];
        _c0[1] = '\0';
	setcolorname( _c0, 2 );
	goto L_8888;

	/* - Option 59: QH --- Select hardware quality text. */

L_5900:
	settexttype( "HARDWARE" );
	goto L_8888;

	/* - Option 60: QS --- Select software quality text. */

L_6000:
	settexttype( "SOFTWARE" );
	goto L_8888;

	/* - Option 61: BH --- Set number of horizontal border tick marks. */
L_6100:
	if( iopei > 0 ){
		cmgam.nhtick = iopei;
		}
	else{
		cmgam.nhtick = 0;
		}
	goto L_8888;

	/* - Option 62: BV --- Set number of vertical border tick marks. */
L_6200:
	if( iopei > 0 ){
		cmgam.nvtick = iopei;
		}
	else{
		cmgam.nvtick = 0;
		}
	goto L_8888;

L_8888:
	return;

} /* end of function */

