
#include "pl.h"
#include "gdm.h"

void /*FUNCTION*/ plhome()
{
	float height, ratio, width;

	/*=====================================================================
	 * PURPOSE:  To home the alphanumeric cursor to its standard position.
	 *=====================================================================
	 * MODULE/LEVEL:  GEM/3
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  GETTEXTSIZE, GETRATIO, MOVE
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    XHOME:   X location in plot coordiates of cursor home.
	 *    YHOME:   Y location in plot coordiates of cursor home.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    870224:  Deleted GEM common block and added inquiry calls.
	 *    830114:  Original version (from PL2D).
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870224
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Home cursor to top left hand corner of screen. */
	gettextsize( &width, &height );
	getratio( &ratio );
	/* xhome = width; */
	/* yhome = ratio - 2.0*height; */
	//move( xhome, yhome );

	return;

} /* end of function */

