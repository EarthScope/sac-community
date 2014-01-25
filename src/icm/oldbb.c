
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ oldbb(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i, npole, nzero;
	float const_;
	complexf pole[5], zero[4];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;




	/*   .....OLD BB - Blacknest specified poles and zeros.....
	 * */


	/*   .....Set poles and zeros.....
	 *
	 *     const = 8.0336077 e1   <*  AMP of 1.0 at FREQ of 1.0 Hz */
	const_ = 8.0336077e1;
	nzero = 4;

	for( i = 1; i <= nzero; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}

	npole = 5;
	Pole[1] = flttocmplx( -0.2140, 0.2300 );
	Pole[2] = flttocmplx( -0.2140, -0.2300 );
	Pole[3] = flttocmplx( -0.3150, 0.0 );
	Pole[4] = flttocmplx( -80.0000, 0.0 );
	Pole[5] = flttocmplx( -0.0555, 0.0 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

