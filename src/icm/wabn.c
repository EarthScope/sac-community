
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ wabn(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int npole, nzero;
	float const_;
	complexf pole[2], zero[2];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....WABN - WOOD-ANDERSON: Blacknest specified poles and zeros.....
	 * */


	/*   .....Set poles and zeros.....
	 *
	 *     const = 2.077594       <*    AMP of 1.0 at FREQ of 1.0 Hz */
	const_ = 2.077594;
	nzero = 2;
	Zero[1] = flttocmplx( 0.0, 0.0 );
	Zero[2] = flttocmplx( 0.0, 0.0 );

	npole = 2;
	Pole[1] = flttocmplx( -6.28318, 4.71239 );
	Pole[2] = flttocmplx( -6.28318, -4.71239 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

