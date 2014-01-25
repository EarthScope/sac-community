
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ benbog(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i, npole, nzero;
	float const_;
	complexf pole[8], zero[4];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....BEN BOG - Blacknest specified poles and zeros.....
	 * */


	/*   .....Set poles and zeros.....
	 *
	 *                             <*  AMP of 1.0 at FREQ of 1.0 Hz */
	const_ = 7.5111330e6;
	nzero = 4;
	for( i = 1; i <= nzero; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}

	npole = 8;
	Pole[1] = flttocmplx( -10.539, 0.0 );
	Pole[2] = flttocmplx( -5.787, 7.407 );
	Pole[3] = flttocmplx( -5.787, -7.407 );
	Pole[4] = flttocmplx( -22.21, 22.21 );
	Pole[5] = flttocmplx( -22.21, -22.21 );
	Pole[6] = flttocmplx( -0.06283, 0.0 );
	Pole[7] = flttocmplx( -29.62, 29.62 );
	Pole[8] = flttocmplx( -29.62, -29.62 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

