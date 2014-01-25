
#include <math.h>

#include "sam.h"

void /*FUNCTION*/ spcval(nx, x, freq, xr, xi, yr, yi)
int nx;
float x[];
double freq;
float *xr, *xi, *yr, *yi;
{
	int j;
	float xj;
	double a, a1, a2, b, c, ca0, d, sa0, u0, u1, u2, w0, w1, w2;

	float *const X = &x[0] - 1;


	/*=====================================================================
	 * PURPOSE:
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:
	 *=====================================================================
	 * GLOBAL INPUT:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *=====================================================================
	 * GLOBAL COUPLING:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * ASSUMPTIONS:
	 *=====================================================================
	 * LIMITATIONS:
	 *=====================================================================
	 * KNOWN ERRORS:
	 *=====================================================================
	 * EXTERNAL DEPENDENCIES:
	 *===================================================================== */
	/* PROCEDURE:
	 *
	 *-----------------------------------------------------------------------
	 * SUBROUTINE: SPCVAL
	 * SUBROUTINE TO COMPUTE A SPECTRAL VALUE AT FREQUENCY
	 * FREQ(RADIANS) FOR SEQUENCES X(N) AND N*X(N)
	 *-----------------------------------------------------------------------
	 * */

	/* DESCRIPTION OF ARGUMENTS:
	 *
	 *   NX = # VALUES IN SEQUENCE X(N)
	 *    X = ARRAY CONTAINING INPUT SEQUENCE X(N)
	 * FREQ = FREQUENCY(RADIANS)
	 *   XR = REAL PART OF THE SPECTRAL VALUE OF X(N)
	 *   XI = IMAGINARY PART OF THE SPECTRAL VALUE OF X(N)
	 *   YR = REAL PART OF THE SPECTRAL VALUE OF NX(N)
	 *   YI = IMAGINARY PART OF THE SPECTRAL VALUE OF NX(N)
	 *
	 * METHOD:
	 *--MODIFIED GOERTZEL ALGORITHM AS PROPOSED BY BONZANIGO
	 *
	 * (IEEE TRAS. ASSP,VOL. 26,NO. 1,FEB 78)
	 *
	 * INITIALIZATION
	 * */
	ca0 = (double)( cos( freq ) );
	sa0 = (double)( sin( freq ) );
	a1 = 2.e0*ca0;
	u1 = 0.e0;
	u2 = u1;
	w1 = u1;
	w2 = u1;

	/* MAIN LOOP (GOERTZEL ALGORITHM)
	 * */
	for( j = 1; j <= nx; j++ ){
		xj = (double)( X[j] );
		u0 = xj + a1*u1 - u2;
		w0 = (double)( (float)( j - 1 ) )*xj + a1*w1 - w2;
		u2 = u1;
		u1 = u0;
		w2 = w1;
		w1 = w0;
		}

	/* BONZANIGO'S PHASE CORRECTION
	 * */
	a = u1 - u2*ca0;
	b = u2*sa0;
	c = w1 - w2*ca0;
	d = w2*sa0;
	a2 = (double)( freq*(float)( nx - 1 ) );
	u1 = cos( a2 );
	u2 = -sin( a2 );
	*xr = (float)( u1*a - u2*b );
	*xi = (float)( u2*a + u1*b );
	*yr = (float)( u1*c - u2*d );
	*yi = (float)( u2*c + u1*d );
	return;
	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    810000:  Original version.
	 *===================================================================== */

} /* end of function */

