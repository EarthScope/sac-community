
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ wwlpbn(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int npole, nzero;
	float const_;
	complexf pole[4], zero[3];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*  .....WWSSN LP - Blacknest specified poles and zeros.....
	 * */


	/*   .....Set poles and zeros.....
	 *
	 *     const = 0.5985275       <*    AMP of 1.0 at FREQ of 0.05 Hz */
	const_ = 0.5985275;

	nzero = 3;
	Zero[1] = flttocmplx( 0.0, 0.0 );
	Zero[2] = flttocmplx( 0.0, 0.0 );
	Zero[3] = flttocmplx( 0.0, 0.0 );

	npole = 4;
	Pole[1] = flttocmplx( -0.257, 0.3376 );
	Pole[2] = flttocmplx( -0.257, -0.3376 );
	Pole[3] = flttocmplx( -0.06283, 0.0 );
	Pole[4] = flttocmplx( -0.06283, 0.0 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

