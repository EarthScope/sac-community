
#include <string.h>

#include "xyz.h"

#include "contouring.h"


#include "cpf.h"

void /*FUNCTION*/ xzticks(nerr)
int *nerr;
{
	int ltemp;
	int j;



	/*=====================================================================
	 * PURPOSE:  To execute the parameter-setting command ZTICKS.
	 *           This command controls contour tick mark annotation on
	 *           subsequent contour plots.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *   NERR:   0 - no error, .ne. 0 - error
	 *=====================================================================
	 * MODULE/LEVEL:  xyz/2
	 *=====================================================================
	 * GLOBAL INPUT: 
	 *   mach:
	 *   contouring:  MZLEVELS, MACTIONTICK
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *   contouring:  ktickmode, iticklist, nticklist,
	 *                ticklength, tickspacing, lticksdown
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:  lcmore, lkia, lkra
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900430:  Rewrote syntax for LIST option.
	 *    900407:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900407
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Parse positional tokens here. */

	/* -- "ON|OFF": Turn tick mark annotation on or off. */
	if( lclog( &ltemp ) ){
		if( ltemp ){
			strcpy( kmcontouring.ktickmode, "ON      " );
			}
		else{
			strcpy( kmcontouring.ktickmode, "OFF     " );
			}
		}

	/* - Loop on remaining tokens in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "LIST n1 n2 ...": Set list of tick attributes. */
		if( lckey( "LIST$",6 ) ){
			cmcontouring.nticklist = 0;
			}
		else if( lckey( "ON#$",5 ) ){
			cmcontouring.nticklist = cmcontouring.nticklist + 1;
			Iticklist[cmcontouring.nticklist] = 1;
			}
		else if( lckey( "OF#F$",6 ) ){
			cmcontouring.nticklist = cmcontouring.nticklist + 1;
			Iticklist[cmcontouring.nticklist] = 0;

			/* -- "DIRECTION UP|DOWN":  Set direction for tick marks. */
			}
		else if( lklog2( "DIRECTION$",11, "DOWN",5, "UP",3, &cmcontouring.lticksdown ) ){

			/* -- "SPACING v":  Set spacing between adjacent tick marks. */
			}
		else if( lkrrc( "SPACING$",9, 0.0, 1.0, &cmcontouring.tickspacing ) ){

			/* -- "LENGTH v":  Set length of tick marks. */
			}
		else if( lkrrc( "LE#NGTH$",9, 0.0, 1.0, &cmcontouring.ticklength ) ){

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

	/* - Reset tick action list. */

	for( j = 1; j <= cmcontouring.nticklist; j++ ){
		if( Iticklist[j] != 0 ){
			if( cmcontouring.lticksdown ){
				Iticklist[j] = -MACTIONTICK;
				}
			else{
				Iticklist[j] = MACTIONTICK;
				}
			}
		}

       
	return;

} /* end of function */

