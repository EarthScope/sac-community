
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ ykasp(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i, npole, nzero;
	float const_;
	complexf pole[6], zero[4];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*  .....YKA SP - Blacknest specified poles and zeros.....
	 * */


	/*   .....Set poles and zeros.....
	 *
	 *     const = 3.8605143 e5      <*   Amp of 1.0 at Freq of 1.0 Hz */
	const_ = 3.8605143e5;

	nzero = 4;
	for( i = 1; i <= nzero; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}

	npole = 6;
	Pole[1] = flttocmplx( -3.830, 4.980 );
	Pole[2] = flttocmplx( -3.830, -4.980 );
	Pole[3] = flttocmplx( -88.700, 88.700 );
	Pole[4] = flttocmplx( -88.700, -88.700 );
	Pole[5] = flttocmplx( -0.628, 0.0 );
	Pole[6] = flttocmplx( -125.66, 0.0 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

