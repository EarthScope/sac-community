
#include "sss.h"
#include "bool.h"


#include "cpf.h"

void /*FUNCTION*/ xtimewindow(nerr)
int *nerr;
{
	int ntused;
  double tmp[2];


	/*=====================================================================
	 * PURPOSE: To parse the parameter-setting command TIMEWINDOW.
	 *          This command defines the "time window" for SSS.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL:  sss/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    sss:     twlim, ltwlim
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, cfmt, cresp, lcra
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    860306:  Changed name from XSTW to XTW.
	 *    821207:  Original version from STMCOM.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850819
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "v1 v2":  set stack window parameters. */
		if( lcra( 2, 2, tmp, &ntused ) ){
			cmsss.ltwlim = TRUE;
      cmsss.twlim[0] = (float) tmp[0];
      cmsss.twlim[1] = (float) tmp[1];
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

       
	return;

} /* end of function */

