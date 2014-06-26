
#include "gtm.h"
#include "debug.h"
void 
worldpolyline(float *xwloc, float *ywloc, int number)
{
	int j;

	/*=====================================================================
	 * PURPOSE:  To draw a line through a set of world locations.
	 *=====================================================================
	 * SPECIAL NOTE:  This polyline subroutine does NOT conform to the
	 *                SIGGRAPH standard in that it moves to the first
	 *                data point as opposed to drawing to it from the CP.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    xwloc:   Array of x world coordinates. [fa]
	 *    ywloc:   Array of y world coordinates. [fa]
	 *    number:  Length of xwloc and ywloc arrays. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    831026:  Original version.
   *    140624:  Changed to use polyline
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861017
	 *===================================================================== */
	/* PROCEDURE: */

  float *x = (float *) malloc(sizeof(float) * number);
  float *y = (float *) malloc(sizeof(float) * number);
  
  /* Coordinate change from world to view */
  for(j = 0; j < number; j++) {
    x[j] = cmgtm.xmpwv1 * xwloc[j] + cmgtm.xmpwv2;
    y[j] = cmgtm.ympwv1 * ywloc[j] + cmgtm.ympwv2;
  }

  polyline(x, y, &number);

  FREE(x);
  FREE(y);

	return;

} /* end of function */

