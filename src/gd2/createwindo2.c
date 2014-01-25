
#include "gd2.h"
#include "debug.h"

void /*FUNCTION*/ createwindow2(number, xwinmn, xwinmx, ywinmn, ywinmx, 
	 nerr)
int *number;
float *xwinmn, *xwinmx, *ywinmn, *ywinmx;
int *nerr;
{

	/*=====================================================================
	 * PURPOSE: To create a new graphics window for device 2 (SGF).
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    NUMBER:  Number of graphic window to "turn on". [i]
	 *    XWINMN:  Minimum x location of window on screen. [f]
	 *    XWINMX:  Maximum x location of window on screen. [f]
	 *             Units for x locations is 0.0 (left) to 1.0 (right).
	 *    YWINMN:  Minimum y location of window on screen. [f]
	 *    YWINMX:  Maximum y location of window on screen. [f]
	 *             Units for y locations are 0.0 (bottom) to RATIO (top),
	 *             where RATIO is the screen aspect ratio obtained
	 *             by a call to GETSCREENRATIO.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred
	 *             Potential error numbers: 0201.
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861201:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850506
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - This is a no-op for this graphics device. */
  UNUSED(number);
  UNUSED(xwinmn);
  UNUSED(xwinmx);
  UNUSED(ywinmn);
  UNUSED(ywinmx);

       
	return;

} /* end of function */

