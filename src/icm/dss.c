
#include "icm.h"
#include "co.h"
#include "complex.h"

void /*FUNCTION*/ dss(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i, npole, nzero;
	float const_, omo;
	complexf crad, pole[8], zero[3];
	static double twopi = 6.283185307179586;

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....LLL DSS - For LLL digital network.....
	 *      ( poles and zeros due to P. Rodgers )
	 * */



	/*   .....Set poles and zeros.....
	 * */
	omo = twopi/30.0;
	const_ = 6.152890842e10;
	nzero = 3;
	for( i = 1; i <= nzero; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}

	npole = 8;
	crad = cmplxsqrt( flttocmplx( 1.0 - powi(0.707,2), 0.0 ) );
	Pole[1] = cmplxmul(flttocmplx(omo,0.),(cmplxadd(flttocmplx(-0.707,0.),cmplxmul(flttocmplx( 0.0, 
	 1.0 ),crad))));
	Pole[2] = cmplxmul(flttocmplx(omo,0.),(cmplxsub(flttocmplx(-0.707,0.),cmplxmul(flttocmplx( 0.0, 
	 1.0 ),crad))));
	Pole[3] = flttocmplx( -33.80165, 60.35942 );
	Pole[4] = flttocmplx( -33.80165, -60.35942 );
	Pole[5] = flttocmplx( -50.43199, 35.28386 );
	Pole[6] = flttocmplx( -50.43199, -35.28386 );
	Pole[7] = flttocmplx( -57.07708, 11.65531 );
	Pole[8] = flttocmplx( -57.07708, -11.65531 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

