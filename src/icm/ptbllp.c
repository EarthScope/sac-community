
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ ptbllp(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i, npole, nzero;
	float const_;
	complexf pole[8], zero[5];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....PTBL LP - Blacknest specified poles and zeros.....
	 * */


	/*   .....Set poles and zeros.....
	 *
	 *     const = 2.554443      <*  AMP of 1.0 at FREQ of O.O5 Hz */
	const_ = 2.554443;
	nzero = 5;

	for( i = 1; i <= nzero; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}

	npole = 8;
	Pole[1] = flttocmplx( -0.2930, 0.2950 );
	Pole[2] = flttocmplx( -0.2930, -0.2950 );
	Pole[3] = flttocmplx( -0.0256, 0.0439 );
	Pole[4] = flttocmplx( -0.0256, -0.0439 );
	Pole[5] = flttocmplx( -0.0417, 0.0417 );
	Pole[6] = flttocmplx( -0.0417, -0.0417 );
	Pole[7] = flttocmplx( -6.2800, 0.0 );
	Pole[8] = flttocmplx( -0.5700, 0.0 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

