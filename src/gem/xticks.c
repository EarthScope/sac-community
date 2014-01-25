
#include "gem.h"
#include "bool.h"


#include "cpf.h"

void /*FUNCTION*/ xticks(nerr)
int *nerr;
{
	int ltf;
	int index;



	/*=====================================================================
	 * PURPOSE: To parse the parameter-setting command TICKS.
	 *          This command sets tick mark drawing attributes.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL:  GEM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GEM:     KSIDES
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GEM:     LTOPTC, LBOTTC, LRIGTC, LLEFTC
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LCLOG, LCKEY, LCLIST
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Assume user wants to turn axes on. */

	ltf = TRUE;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- Set flag so that only listed axes are turned on. */
		if( lckey( "ONL$",5 ) ){
			cmgem.axis[TOP].ticks    = FALSE;
			cmgem.axis[BOTTOM].ticks = FALSE;
			cmgem.axis[RIGHT].ticks  = FALSE;
			cmgem.axis[LEFT].ticks   = FALSE;
			ltf = TRUE;

			/* -- Set flag to either turn axes on or off. */
			}
		else if( lclog( &ltf ) ){

			/* --- A particular axes name to turn on or off. */
			}
		else if( lclist( (char*)kmgem.ksides,9, 4, &index ) ){
                  cmgem.axis[index].ticks = ltf;
                }
		else if( lckey( "A$",3 ) ){
			cmgem.axis[BOTTOM].ticks = ltf;
			cmgem.axis[LEFT].ticks   = ltf;
			cmgem.axis[RIGHT].ticks  = ltf;
			cmgem.axis[TOP].ticks    = ltf;

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
	 *    820610:  Original version.
	 *===================================================================== */

} /* end of function */

