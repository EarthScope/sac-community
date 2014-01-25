
#include "icm.h"
#include "co.h"
#include "complex.h"

void /*FUNCTION*/ general(nfreq, delfrq, xre, xim, nzer, t0, h, const_)
int nfreq;
double delfrq, xre[], xim[];
int nzer;
double t0, h, const_;
{
	int i, npole, nzero;
	float omo;
	complexf crad, pole[2], zero[3];
	static double twopi = 6.283185307179586;

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....GENERAL - for a general seismometer.....
	 * */



	/*   .....Set poles and zeros.....
	 * */
	nzero = 3;
	for( i = 1; i <= nzero; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}
	nzero = nzer;

	npole = 2;
	omo = twopi/t0;
	crad = cmplxsqrt( flttocmplx( 1.0 - powi(h,2), 0.0 ) );
	Pole[1] = cmplxmul(flttocmplx(omo,0.),(cmplxadd(flttocmplx(-h,0.),cmplxmul(flttocmplx( 0.0, 
	 1.0 ),crad))));
	Pole[2] = cmplxmul(flttocmplx(omo,0.),(cmplxsub(flttocmplx(-h,0.),cmplxmul(flttocmplx( 0.0, 
	 1.0 ),crad))));

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

