
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ wwspbn(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int npole, nzero;
	float const_;
	complexf pole[5], zero[3];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*  .....WWSSN SP - Blacknest specified poles and zeros.....
	 * */


	/*   .....Set poles and zeros.....
	 * */
	const_ = 432.83395;

	nzero = 3;
	Zero[1] = flttocmplx( 0.0, 0.0 );
	Zero[2] = flttocmplx( 0.0, 0.0 );
	Zero[3] = flttocmplx( 0.0, 0.0 );

	npole = 5;
	Pole[1] = flttocmplx( -4.04094, 6.47935 );
	Pole[2] = flttocmplx( -4.04094, -6.47935 );
	Pole[3] = flttocmplx( -9.25238, 0.0 );
	Pole[4] = flttocmplx( -7.67430, 0.0 );
	Pole[5] = flttocmplx( -16.72981, 0.0 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

