#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mach.h"
#include "gam.h"
#include "bool.h"


#include "cpf.h"

void  xfilenumber(int *nerr)
{
	int lfino = FALSE ;



	/*=====================================================================
	 * PURPOSE: To parse the parameter-setting command FILENUMBER.
	 *          This command controls the file number display on each plot.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL:  GAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GAM:     LFINORQ
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GAM:     LFINORQ
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LCLOG
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

		/* -- "ON/OFF":  turn file number display on or off. */
		if( lclog( &lfino ) )
		{ /* do nothing */ }


		/* -- Bad syntax. */
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();
		}

	} /* end while ( lcmore( nerr ) ) */

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	cmgam.lfinorq = lfino ;

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    970204:  Original version (from XFID).
	 *===================================================================== */

} /* end of function */

