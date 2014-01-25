#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gcm.h"
#include "bool.h"


#include "gdm.h"
#include "cpf.h"

void /*FUNCTION*/ xvspac(nerr)
int *nerr;
{
	double ratio;

	/*=====================================================================
	 * PURPOSE: To parse the parameter-setting command VSPACE.
	 *          This command defines the graphics viewspace.
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
	 *    SACLIB:  LCMORE, CFMT, CRESP, LCKEY, LCREAL,
	 *             SETVSPACETYPE, CALVSPACE
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861112:  Major rewrite due to new graphics library.
	 *    840912:  Added ability to explicitely set viewspace ratio.
	 *    811007:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861112
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "FULL": select full viewspace option. */
		if( lckey( "FULL$",6 ) ){
			setvspacetype( TRUE, 1.0 );

			/* -- "v":  set viewspace ratio directly. */
			}
		else if( lcreal( &ratio ) ){
			setvspacetype( FALSE, (float)ratio );

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

	/* - Calculate the new viewspace. */

	calvspace();

       
	return;

} /* end of function */

