#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "gam.h"


#include "cpf.h"

void xylim(int *nerr)
{
	int j, j_, jl;
	double real1, real2, realv;



	/*=====================================================================
	 * PURPOSE: To parse the parameter-setting command YLIM.
	 *          This command sets y axis plot limits (world coordinates.)
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL:  GAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GAM:     MYLIM
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GAM:     NYLIM, KYLIMS, YLIMS
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CERR, LCLOG, LCREAL
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    870728:  Added storage of nylim.
	 *    820610:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870728
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Initialize counter. */

	jl = 1;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- Turn y limits on, off, or scaled to all data. */
		if( lckey( "ON$",4 ) ){
			strcpy( kmgam.kylims[jl - 1], "ON      " );
			jl = jl + 1;

			}
		else if( lckey( "OF$",4 ) ){
			strcpy( kmgam.kylims[jl - 1], "OFF     " );
			jl = jl + 1;

			}
		else if( lckey( "A$",3 ) ){
			strcpy( kmgam.kylims[jl - 1], "ALL     " );
			jl = jl + 1;

			/* -- Set y limits to plus and minus input value. */
			}
		else if( lkreal( "+-$",4, &realv ) || lkreal( "PM$",4, &realv ) ){
			strcpy( kmgam.kylims[jl - 1], "ON      " );
			cmgam.ylims[jl - 1][0] = -fabs( realv );
			cmgam.ylims[jl - 1][1] = fabs( realv );
			jl = jl + 1;

			/* -- Set y limits to a pair of real variables. */
			}
		else if( lcreal( &real1 ) && lcreal( &real2 ) ){
			strcpy( kmgam.kylims[jl - 1], "ON      " );
			cmgam.ylims[jl - 1][0] = (float)real1;
			cmgam.ylims[jl - 1][1] = (float)real2;
			jl = jl + 1;

			/* -- Bad syntax. */
			}
		else{
			cerr( 1001 );

			}
		goto L_1000;
		}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
		goto L_8888;

	/* - Set remainder of y limit values to last one input. */

	cmgam.nylim = jl - 1;
	for( j = cmgam.nylim + 1; j <= MYLIM; j++ ){
		j_ = j - 1;
		strcpy( kmgam.kylims[j_], kmgam.kylims[cmgam.nylim - 1] );
		cmgam.ylims[j_][0] = cmgam.ylims[cmgam.nylim - 1][0];
		cmgam.ylims[j_][1] = cmgam.ylims[cmgam.nylim - 1][1];
		}

L_8888:
	return;

} /* end of function */

