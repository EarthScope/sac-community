#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gam.h"
#include "gdm.h"
#include "msg.h"
#include "cpf.h"

void xgamc(int index, int *nerr)
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
	 * MODULE/LEVEL: GAM/1
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GAM:     LRTWXL, KRTWXL, ORTWXL
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  GTOUTM, XP, XP1, XP2, XPPK, XPC, XFID, XPICKS, XPLOTPM
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    970204:  Added FILENUMBER to allow or disallow file number
	 *             display.  maf
	 *    890421:  Added PLOTXY command.
	 *    870728:  Moved XLIM and YLIM commands from GEM.
	 *    870420:  Added SETDEVICE command.
	 *    861203:  Added PLOTPM command.
	 *    830105:  Deleted PUSER command.
	 *    821228:  Added FILEID and PICKS commands. Deleted DISPLAY command.
	 *    821005:  Added PLOTC command.
	 *    820825:  Original version.
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
		case 16: goto L_1600;	/* added for FILENUMBER. maf 970204 */
		case 17: goto L_1700;	/* added for PRINT. maf 990422 */
                case 18: goto L_1800;
		}

	/* - Error return if bad index value. */

	*nerr = 901;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XGAMC",9 );
	goto L_8888;

L_100:
	/* - Command 01: PLOT */
	xp( nerr );
	goto L_8888;

L_200:
	/* - Command 02: PLOT1 */
	xp1( nerr );
	goto L_8888;

L_300:
	/* - Command 03: PLOT2 */
	xp2( nerr );
	goto L_8888;

L_400:
	/* - Command 04: PLOTPK */
	xppk( nerr );
	goto L_8888;

L_500:
	/* - Command 05: PLOTC */
	xpc( nerr );
	goto L_8888;

L_600:
	/* - Command 06: FILEID */
	xfid( nerr );
	goto L_8888;

L_700:
	/* - Command 07: PICKS */
	xpicks( nerr );
	goto L_8888;

L_800:
	/* - Command 08: PLOTPM */
	xplotpm( nerr );
	goto L_8888;

L_900:
	/* - Command 09: SETDEVICE */
	xsetdevice( nerr );
	goto L_8888;

L_1000:
	/* - Command 10: XLIM */
	xcrtw( &cmgam.lrtwxl, (char*)kmgam.krtwxl,9, cmgam.ortwxl, nerr );
	goto L_8888;

L_1100:
	/* - Command 11: YLIM */
	xylim( nerr );
	goto L_8888;

L_1200:
	/* - Command 12: PLOTXY */
	xplotxy( nerr );
	goto L_8888;

L_1300:
	/* - Command 13: FITXY */
	xfitxy( nerr );
	goto L_8888;

L_1400:
	/* - Command 14: PLOTDY */
	xplotdy( nerr );
	goto L_8888;

L_1500:
	/* - Command 15: PLOTALPHA */
	xplotalpha( nerr );
	goto L_8888;

L_1600:
	/* - Command 16: FILENUMBER */
	xfilenumber ( nerr ) ;
	goto L_8888 ;

L_1700:
	/* - Command 17: PRINT */
	xprint ( nerr ) ;
	goto L_8888 ;

 L_1800:
        xsave( );
        goto L_8888;

L_8888:
	return;

} /* end of function */

