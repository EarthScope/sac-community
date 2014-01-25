
#include "sss.h"
#include "bool.h"


#include "cpf.h"

void /*FUNCTION*/ xdeltacheck(nerr)
int *nerr;
{


  double tmp;
	/*=====================================================================
	 * PURPOSE: To parse the parameter-setting command DELTACHECK
	 *          This command controls the sampling rate checking done in SSS.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL:  sss/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    rndoff
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    sss:     lsrc, srcfac
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, cfmt, cresp, lclog, lckey, lcreal
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    850819:  Named changed from XSRCHK to XDCH.
	 *    821207:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850819
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "ON/OFF":  turn sampling rate checking on or off. */
		if( lclog( &cmsss.lsrc ) ){

			/* -- "ROUNDOFF": set sampling rate check value to machine roundoff. */
			}
		else if( lckey( "ROUNDOFF$",10 ) ){
			cmsss.lsrc = TRUE;
			cmsss.srcfac = RNDOFF;

			/* -- "v":  set sampling rate check value to fixed value. */
			}
		else if( lcreal( &tmp ) ){
			cmsss.lsrc = TRUE;
      cmsss.srcfac = (float) tmp;
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

