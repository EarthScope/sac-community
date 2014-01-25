
#include <string.h>

#include "xyz.h"

#include "contouring.h"


#include "cpf.h"

void /*FUNCTION*/ xzlines(nerr)
int *nerr;
{
	int ltemp;



	/*=====================================================================
	 * PURPOSE:  To execute the parameter-setting command ZLINES.
	 *           This command controls contour linestyle for subsequent
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
	 *   contouring:  klinemode, klinetype, linelist, nlinelist,
	 *                zregionlist, nzregionlist
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:  lcmore, lkia, lkra
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900430:  Added option to turn lines on and off.
	 *    900404:  Changed to new contouring module.
	 *    900307:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900404
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Parse positional tokens here. */

	/* -- "ON|OFF": Turn tick mark annotation on or off. */
	if( lclog( &ltemp ) ){
		if( ltemp ){
			strcpy( kmcontouring.klinemode, "ON      " );
			}
		else{
			strcpy( kmcontouring.klinemode, "OFF     " );
			}
		}

	/* - Loop on remaining tokens in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- LIST n1 n2 ...: Set list of contour linestyles. */
		if( lkia( "LIST$",6, 1, MZLEVELS, cmcontouring.linelist, &cmcontouring.nlinelist ) ){
			strcpy( kmcontouring.klinetype, "LIST    " );

			/* -- REGIONS n1 v1 n2 v2 ... nn: Set linestyles for zlevel regions. */
			}
		else if( lkra( "REGIONS$",9, 1, MZLEVELS, cmcontouring.zregionlist, 
		 &cmcontouring.nzregionlist ) ){
			strcpy( kmcontouring.klinetype, "REGIONS " );

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

