
#include "gtm.h"
#include "gdm.h"

void /*FUNCTION*/ rectangle(xloc1, xloc2, yloc1, yloc2)
float *xloc1, *xloc2, *yloc1, *yloc2;
{

	/*=====================================================================
	 * PURPOSE:  To draw a rectangle.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    xloc1:   X viewport location of one corner of rectangle. [f]
	 *    xloc2:   X viewport location of other corner. [f]
	 *    yloc1:   Y viewport location of one corner of rectangle. [f]
	 *    yloc2:   Y viewport location of other corner. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  move, draw
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    810000:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861020
	 *===================================================================== */
	/* PROCEDURE: */
	move( *xloc1, *yloc1 );
	draw( *xloc1, *yloc2 );
	draw( *xloc2, *yloc2 );
	draw( *xloc2, *yloc1 );
	draw( *xloc1, *yloc1 );
        stroke();

	return;

} /* end of function */

