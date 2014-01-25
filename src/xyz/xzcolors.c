
#include <string.h>

#include "mach.h"
#include "xyz.h"

#include "contouring.h"


#include "co.h"
#include "ucf.h"
#include "cpf.h"

void /*FUNCTION*/ xzcolors(nerr)
int *nerr;
{
	char klist[MCMSG+1];
	int llabelmode;
	int ic, ic1, ic2, itype, j, nclist;



	/*=====================================================================
	 * PURPOSE:  To execute the parameter-setting command ZCOLORS.
	 *           This command controls contour line colors for
	 *           subsequent contour plots.
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
	 *   contouring:  kcolormode, kcolorlist, ncolorlist
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:  lcmore, lclog, lkrest
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900430:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900430
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Parse positional tokens here. */

	/* -- "ON|OFF":  turn line labeling on or off. */
	if( lclog( &llabelmode ) ){
		if( llabelmode ){
			strcpy( kmcontouring.kcolormode, "ON      " );
			}
		else{
			strcpy( kmcontouring.kcolormode, "OFF     " );
			}
		}

	/* - Loop on remaining tokens in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "LIST colorname ...":  set list of line color attributes. */
		if( lkrest( "LIST$",6, MCMSG, klist,MCMSG+1, &nclist ) ){
			j = 0;
			ic = 0;
L_1100:
			poptok( klist, nclist, &ic, &ic1, &ic2, &itype );
			if( itype > 0 ){
				j = min( j + 1, MZLEVELS );
/*     		        fstrncpy( kmcontouring.kcolorlist[j - 1],  */
/* 			  klist+ic1 - 1,min(ic2,MCMSG) - ic1 + 1); */
			fstrncpy( kmcontouring.kcolorlist[j - 1], 
				  9,
				  klist+ic1 - 1,
				  min(ic2,MCMSG) - ic1 + 1
				  );
				goto L_1100;
				}
			cmcontouring.nlabellist = j;

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

