
#include "gtm.h"
#include "gdm.h"

void /*FUNCTION*/ line(xloc1, yloc1, xloc2, yloc2)
float xloc1, yloc1, xloc2, yloc2;
{

	/*=====================================================================
	 * PURPOSE:  To draw a line between two viewport coordinates.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    xloc1:   First X viewport coordinate. [f]
	 *    yloc1:   First Y viewport coordinate. [f]
	 *    xloc2:   Second X viewport coordinate. [f]
	 *    yloc2:   Second Y viewport coordinate. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *   saclib:   move, draw
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    831026:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861017
	 *===================================================================== */
	/* PROCEDURE: */

	move( xloc1, yloc1 );
	draw( xloc2, yloc2 );
        stroke();

	return;

} /* end of function */

