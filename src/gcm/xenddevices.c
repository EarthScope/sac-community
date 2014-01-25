#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mach.h"
#include "gcm.h"



#include "gdm.h"
#include "cpf.h"

void /*FUNCTION*/ xenddevices(nerr)
int *nerr;
{
	char kdev[9];
	int nchar;
    memset(kdev, 0, sizeof(kdev));
	/*=====================================================================
	 * PURPOSE:  To execute the action command ENDDEVICES.
	 *           This command terminates one or more graphics devices.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL: OFM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:    MCPW
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, LCCHAR, CERR, CFMT, CRESP, ENDGRAPHICS, ENDDEVICE
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    850404:  Moved name checking logic to ENDDEVICE.
	 *    820817:  Changed to newest set of parsing and checking functions.
	 *    811001:  Replaced ZPLACT with ZENDDEVICES.
	 *    810120:  Changed to output message retrieval from disk.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861112
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		if( lckey( "ALL#$",6 ) ){
			endgraphics( nerr );
			if( *nerr != 0 )
				goto L_8888;

			/* -- Call graphics device handler with each token. */
			}
		else if( lcchar( MCPW, kdev,9, &nchar ) ){
			enddevice( kdev,9, nerr );
			if( *nerr != 0 )
				goto L_8888;

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

L_8888:
	return;

} /* end of function */

