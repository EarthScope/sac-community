
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ hfslpwb(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i, npole, nzero;
	float const_;
	complexf pole[6], zero[4];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....HFS LPWB - Blacknest specified poles and zeros.....
	 * */


	/*   .....Set poles and zeros.....
	 *
	 *                            <*  AMP of 1.0 at FREQ of 0.05 Hz */
	const_ = 0.1761249;
	nzero = 4;
	for( i = 1; i <= nzero; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}

	npole = 6;
	Pole[1] = flttocmplx( -0.1100, 0.2380 );
	Pole[2] = flttocmplx( -0.1100, -0.2380 );
	Pole[3] = flttocmplx( -0.1340, 0.1605 );
	Pole[4] = flttocmplx( -0.1340, -0.1605 );
	Pole[5] = flttocmplx( -0.0312, 0.0 );
	Pole[6] = flttocmplx( -0.6450, 0.0 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

