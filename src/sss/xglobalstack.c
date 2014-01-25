#include "sss.h"
#include "bool.h"

#include "cpf.h"

void xglobalstack(int *nerr)
{
	double delay, tmp;

	/*=====================================================================
	 * PURPOSE: To parse the parameter-setting command GLOBALSTACK.
	 *          This command defines global stack file list properties.
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
	 *    sss:     wtg, ldlyt, ldlyt, dlytg, dlytig, dlyng, dlynig,
	 *             lpolg, dstg
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, cfmt, cresp, lkreal, lclog2, lkchar
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    850812:  Major revision.
	 *    821207:  Original version from STMCOM.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850812
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Loop on each token in command: */

	while( lcmore( nerr ) ){

		/* -- "WEIGHT v":  define global weight property. */
		if( lkreal( "WEIGHT$",8, &delay ) ){
      cmsss.wtg = (float) delay;
			/* -- "DELAY v":  define global static delay propertys. */
			}
		else if( lkreal( "DE#LAY$",8, &delay ) ){
			if( lckey( "SECONDS$",9 ) ){
				cmsss.dlytg = delay;
				}
			else if( lckey( "POINTS$",8 ) ){
				cmsss.dlyng = delay;
				}

			/* -- "INCREMENT v":  define global static delay propertys. */
			}
		else if( lkreal( "INCREMENT$",11, &delay ) ){
			if( lckey( "SECONDS$",9 ) ){
				cmsss.dlytig = delay;
				}
			else if( lckey( "POINTS$",8 ) ){
				cmsss.dlynig = delay;
				}

			/* -- "NORMAL/REVERSED":  define global polarity property. */
			}
		else if( lclog2( "NORMAL$",8, "REVERSED$",10, &cmsss.lpolg ) ){

			/* -- "DISTANCE v":  define global distance property. */
			}
		else if( lkreal( "DI#STANCE$",11, &tmp ) ){
      cmsss.dstg = (float) tmp;
			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}

		}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

       
	return;

} /* end of function */

