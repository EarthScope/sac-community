
#include "xyz.h"


#include "msg.h"

void /*FUNCTION*/ xxyzc(index, nerr)
int index, *nerr;
{

	/*=====================================================================
	 * PURPOSE: To execute commands in the XYZ Data Module.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    index:   Index number of the command to execute. [i]
	 *=====================================================================
	 *    nerr:    Error flag. Set to 0 if no error occurred. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  xyz/4
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900430:  Added ZCOLORS command.
	 *    900425:  Added ZLABELS command.
	 *    900406:  Added ZTICKS command.
	 *    900307:  Added ZLEVELS and ZLINES command.
	 *    900305:  Changed module name from imaging to xyz.
	 *             Added contouring commands.
	 *    900129:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900305
	 *===================================================================== */
	/* LOCAL VARIABLES  */
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
		}

	/* - Error return if bad index value. */

	*nerr = 901;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XXYZC",9 );
	goto L_8888;

	/* - Command 01: SPECTROGRAM */

L_100:
	xspectrogram( nerr );
	goto L_8888;

	/* - Command 02: GRAYSCALE */

L_200:
	xgrayscale( nerr );
	goto L_8888;

	/* - Command 03: CONTOUR */

L_300:
	xcontour( nerr );
	goto L_8888;

	/* - Command 04: ZLEVELS */

L_400:
	xzlevels( nerr );
	goto L_8888;

	/* - Command 05: ZLINES */

L_500:
	xzlines( nerr );
	goto L_8888;

	/* - Command 06: ZTICKS */

L_600:
	xzticks( nerr );
	goto L_8888;

	/* - Command 07: ZLABELS */

L_700:
	xzlabels( nerr );
	goto L_8888;

	/* - Command 08: ZCOLORS */

L_800:
	xzcolors( nerr );
	goto L_8888;

L_900:
        ximage( nerr );
        goto L_8888;

L_1000:
        xscallop( nerr );
        goto L_8888;

L_8888:
	return;

} /* end of function */

