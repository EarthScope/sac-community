
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ gbalp(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i, npole, nzero;
	float const_;
	complexf pole[10], zero[4];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....GBA LP - Blacknest specified poles and zeros.....
	 * */


	/*   .....Set poles and zeros.....
	 *
	 *                                <*  AMP of 1.0 at FREQ of 0.05 Hz */
	const_ = 0.100844452e-1;
	nzero = 4;
	for( i = 1; i <= nzero; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}

	npole = 10;
	Pole[1] = flttocmplx( -0.2140, 0.2300 );
	Pole[2] = flttocmplx( -0.2140, -0.2300 );
	Pole[3] = flttocmplx( -0.1340, 0.1605 );
	Pole[4] = flttocmplx( -0.1340, -0.1605 );
	Pole[5] = flttocmplx( -0.0312, 0.0 );
	/*                                          <* KKN */
	Pole[6] = flttocmplx( -2.060, 0.0 );
	Pole[7] = flttocmplx( -0.1670, 0.2550 );
	Pole[8] = flttocmplx( -0.1670, -0.2550 );
	Pole[9] = flttocmplx( -0.1670, 0.2550 );
	Pole[10] = flttocmplx( -0.1670, -0.2550 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

