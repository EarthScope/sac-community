
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ gbasp(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i, npole, nzero;
	float const_;
	complexf pole[4], zero[3];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....GBA SP - Blacknest specified poles and zeros.....
	 * */


	/*   .....Set poles and zeros.....
	 *
	 *                             <*  AMP of 1.0 at FREQ of 1.6 Hz */
	const_ = 2.5597471e2;
	nzero = 3;
	for( i = 1; i <= nzero; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}

	npole = 4;
	Pole[1] = flttocmplx( -4.02, 4.82 );
	Pole[2] = flttocmplx( -4.02, -4.82 );
	Pole[3] = flttocmplx( -40.2, 30.2 );
	Pole[4] = flttocmplx( -40.2, -30.2 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

