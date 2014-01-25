
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ ekasp2(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i, npole, nzero;
	float const_;
	complexf pole[4], zero[4];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....EKA SP2 - Blacknest specified poles and zeros.....
	 * */


	/*   .....Set poles and zeros.....
	 *
	 *                                <*  AMP of 1.0 at FREQ of 1.0 Hz */
	const_ = 12.9083123695;
	nzero = 4;
	for( i = 1; i <= nzero; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}

	npole = 4;
	Pole[1] = flttocmplx( -3.7700, 5.0200 );
	Pole[2] = flttocmplx( -3.7700, -5.0200 );
	Pole[3] = flttocmplx( -67.2000, 0.0 );
	Pole[4] = flttocmplx( -0.3300, 0.0 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

