
#include <math.h>

#include "sss.h"
#include "debug.h"

void /*FUNCTION*/ phaseadj(leven, npts, xarray, yarray, xfirst, xdel, 
	 bdist, dist, atime, nerr)
int leven;
int npts;
float xarray[], yarray[];
double xfirst, xdel, bdist, dist;
float *atime;
int *nerr;
{
	int i, n;
	float deldist;

	float *const Xarray = &xarray[0] - 1;
	float *const Yarray = &yarray[0] - 1;
  UNUSED(bdist);

	/*=====================================================================
	 * PURPOSE:  Adjust the time relative to a certain phase structure. Used to
	 *           create reduced travel time plots.  The phase is defined by 
	 *           xarray and yarray.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *     leven:    Phase information. Indicates whether the phase curve is
	 *               evenly spaced data. (l)
	 *     npts:     Phase information. Indicates number of points in curve. (i)
	 *     xarray:   Phase information. For unevenly spaced data this is an 
	 *               array of x values that defines the travel 
	 *               time curve for this phase (distance). (fa)
	 *     yarray:   Phase information. Array of y values that defines the travel 
	 *               time curve for this phase (time). (fa)
	 *     bdist:    Initial distance (for t0). (f)
	 *     xfirst:   Phase information. For evenly spaced data this is the
	 *               initial distance for the travel time curve (f)
	 *     xdel:     Phase information. For evenly spaced data this is the
	 *               sample rate for the travel time curve (f)
	 *     sdist:    Distance at which to compute the new time. (f)
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

	/* - Compute time */

	if( leven ){
		n = (dist - xfirst)/xdel;
		deldist = fabs( dist - (float)( n )*xdel + xfirst );
		n = n + 1;
		if( deldist < RNDOFF ){
			*atime = Yarray[n];
			}
		else{
			*atime = (Yarray[n + 1] - Yarray[n])/xdel*deldist + Yarray[n];
			}
		}
	else{
		for( i = 1; i <= npts; i++ ){
			if( fabs( Xarray[i] - dist ) < RNDOFF )
				goto L_30;
			if( (i < npts && Xarray[i] <= dist) && Xarray[i + 1] > 
			 dist )
				goto L_40;
			}
		*nerr = 1;
		goto L_8888;
L_30:
		*atime = Yarray[i];
		goto L_8888;
L_40:
		*atime = (Yarray[i + 1] - Yarray[i])/(Xarray[i + 1] - Xarray[i])*
		 (dist - Xarray[i]) + Yarray[i];
		goto L_8888;
		}


L_8888:
	return;

} /* end of function */

