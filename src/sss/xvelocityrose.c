
#include "sss.h"
#include "gam.h"


#include "cpf.h"

void /*FUNCTION*/ xvelocityrose(nerr)
int *nerr;
{



	/*=====================================================================
	 * PURPOSE: To parse the parameter-setting command VELOCITYROSET.
	 *          This command controls placing of velocity roset
	 *          on record section plot.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL:  sss/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    gam:     kfidlc, nfidlc
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    sss:     lroset, iroset
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, cfmt, cresp, lclog, lclist
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    881130:  Used file id locations rather than special SSS ones.
	 *    881116:  Changed syntax slightly.
	 *    821207:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850821
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "ON|OFF":  turn roset plotting on or off. */
		if( lclog( &cmsss.lroset ) ){

			/* -- "LOCATION UL|UR|LL|LR":  change roset location. */
			}
		else if( lclist( (char*)kmgam.kfidlc,9, cmgam.nfidlc, &cmsss.iroset ) ){

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

