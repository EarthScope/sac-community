
#include "gem.h"
#include "bool.h"


#include "cpf.h"

void /*FUNCTION*/ xygrid(nerr)
int *nerr;
{



	/*=====================================================================
	 * PURPOSE: To parse the parameter-setting command YGRID.
	 *          This command controls grid plotting parallel to the y axis.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL:  GEM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GEM:     ISOLID, IDOT
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GEM:     LYGRD, IYGRD
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- Turn gridding on or off. */
		if( lclog( &cmgem.lygrd ) ){

			/* -- Change to solid linestyle. */
			}
		else if( lckey( "S$",3 ) ){
			cmgem.iygrd = LINE_STYLE_SOLID;
			cmgem.lygrd = TRUE;

			/* -- Change to dotted linestyle. */
			}
		else if( lckey( "D$",3 ) ){
			cmgem.iygrd = LINE_STYLE_DOTTED;
			cmgem.lygrd = TRUE;

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

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    850321:  Typing SOLID or DOTTED now turns on gridding.
	 *    820611:  Original version.
	 *===================================================================== */

} /* end of function */

