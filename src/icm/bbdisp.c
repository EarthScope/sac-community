
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ bbdisp(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i, npole, nzero;
	float const_;
	complexf pole[7], zero[3];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....BB DISP - Blacknest specified poles and zeros.....
	 * */


	/*   .....Set poles and zeros.....
	 *
	 *                           <*  AMP of 1.0 at FREQ of 1.0 Hz */
	const_ = 3.690549e5;
	nzero = 3;

	for( i = 1; i <= nzero; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}

	npole = 7;
	Pole[1] = flttocmplx( -0.2140, 0.2300 );
	Pole[2] = flttocmplx( -0.2140, -0.2300 );
	Pole[3] = flttocmplx( -6.4400, 23.4000 );
	Pole[4] = flttocmplx( -6.4400, -23.400 );
	Pole[5] = flttocmplx( -25.0000, 0.0 );
	Pole[6] = flttocmplx( -25.0000, 0.0 );
	Pole[7] = flttocmplx( -0.0555, 0.0 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

