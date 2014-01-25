
#include "sss.h"
#include "debug.h"

void /*FUNCTION*/ velocityadj(velocity, bdist, dist, atime, nerr)
double velocity, bdist, dist;
float *atime;
int *nerr;
{
	float sdist;
  UNUSED(bdist);
	/*=====================================================================
	 * PURPOSE:  Adjust the time relative to a certain velocity. Used to
	 *           create reduced travel time plots.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *     velocity: Velocity.
	 *     bdist:    Initial distance (for t0). (f)
	 *     sdist:    Distance at which to compute the new time. (f)
	 * Shawn says set this to zero to get the right corrections 5/96
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    atime:   Adjusted for this velocity distance
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  sss/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920729:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;
	sdist = 0.;
	/* - Compute adjusted start time */

	*atime = (dist - sdist)/velocity;

       
	return;

} /* end of function */

