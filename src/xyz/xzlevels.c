
#include <string.h>

#include "xyz.h"

#include "contouring.h"


#include "cpf.h"

void /*FUNCTION*/ xzlevels(nerr)
int *nerr;
{
	int nrange;
	double range[2];

	double *const Range = &range[0] - 1;


	/*=====================================================================
	 * PURPOSE:  To execute the parameter-setting command ZLEVELS.
	 *           This command sets the contour levels for subsequent
	 *           contour plots.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *   NERR:   0 - no error, .ne. 0 - error
	 *=====================================================================
	 * MODULE/LEVEL:  xyz/2
	 *=====================================================================
	 * GLOBAL INPUT: 
	 *   mach:
	 *   contouring:  MZLEVELS
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *   contouring:  klevelmode, zlevelminimum, zlevelmaximum,
	 *                zlevelincrement, nznumlevels, 
	 *                zlevellist, nzlevellist
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:  lcmore, lkra, lckey, lkreal, lkirc, cfmt, cresp
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900307:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900307
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- RANGE v1 v2:  Set range of contour levels. */
		if( lkra( "RANGE$",7, 2, 2, range, &nrange ) ){
			strcpy( kmcontouring.klevelmode, "RANGE   " );
			cmcontouring.zlevelminimum = (float)Range[1];
			cmcontouring.zlevelmaximum = (float)Range[2];

			/* -- SCALE:  Scale contour levels to data. */
			}
		else if( lckey( "SCALE$",7 ) ){
			strcpy( kmcontouring.klevelmode, "SCALE   " );

			/* -- INCREMENT v:  Set contour level increment. */
			}
		else if( lkreal( "INCREMENT$",11, &cmcontouring.zlevelincrement ) ){

			/* -- NUMBER n: Set number of contour levels. */
			}
		else if( lkirc( "NUMBER$",8, 1, MZLEVELS, &cmcontouring.nznumlevels ) ){
			cmcontouring.zlevelincrement = -1.0;

			/* -- LIST v1 v2 ...: Set list of contour levels. */
			}
		else if( lkra( "LIST$",6, 1, MZLEVELS, cmcontouring.zlevellist, 
		 &cmcontouring.nzlevellist ) ){
			strcpy( kmcontouring.klevelmode, "LIST    " );

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

