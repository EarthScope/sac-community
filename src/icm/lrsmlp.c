
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ lrsmlp(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i, npole, nzero;
	float const_;
	complexf pole[7], zero[4];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....LRSM LP - Blacknest specified poles and zeros.....
	 * */


	/*   .....Set poles and zeros.....
	 * */
	const_ = 0.65871e-1;
	nzero = 4;

	for( i = 1; i <= nzero; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}

	npole = 7;
	Pole[1] = flttocmplx( -0.19635, 0.24524 );
	Pole[2] = flttocmplx( -0.19635, -0.24524 );
	Pole[3] = flttocmplx( -0.20942, 0.0 );
	Pole[4] = flttocmplx( -0.20942, 0.0 );
	Pole[5] = flttocmplx( -0.00628, 0.0 );
	Pole[6] = flttocmplx( -0.17593, 0.17948 );
	Pole[7] = flttocmplx( -0.17593, -0.17948 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

