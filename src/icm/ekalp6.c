
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ ekalp6(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i, npole, nzero;
	float const_;
	complexf pole[9], zero[6];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....EKA LP6 - Blacknest specified poles and zeros.....
	 * */


	/*   .....Set poles and zeros.....
	 *
	 *                             <*  AMP of 1.0 at FREQ of 0.05 Hz */
	const_ = 0.1084564;
	nzero = 6;
	for( i = 1; i <= 4; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}
	Zero[5] = flttocmplx( -0.00524, 1.04720 );
	Zero[6] = flttocmplx( -0.00524, -1.04720 );

	npole = 9;
	Pole[1] = flttocmplx( -0.29323, 0.29915 );
	Pole[2] = flttocmplx( -0.29323, -0.29915 );
	Pole[3] = flttocmplx( -0.10996, 0.11218 );
	Pole[4] = flttocmplx( -0.10996, -0.11218 );
	Pole[5] = flttocmplx( -0.03140, 0.0 );
	Pole[6] = flttocmplx( -0.22000, 0.22400 );
	Pole[7] = flttocmplx( -0.22000, -0.22400 );
	Pole[8] = flttocmplx( -1.04720, 0.0 );
	Pole[9] = flttocmplx( -1.04720, 0.0 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

