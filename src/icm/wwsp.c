
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ wwsp(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int npole, nzero;
	float const_;
	complexf pole[5], zero[3];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*  WWSSN short period seismometer.
	 *  Ref:  Luh, P. C. (1977).  A scheme for expressing instrumental
	 *  responses parametrically, BSSA, 67, 957-969.
	 * */


	/*   .....Set poles and zeros.....
	 * */
	const_ = 397.54767;
	nzero = 3;
	Zero[1] = flttocmplx( 0.0, 0.0 );
	Zero[2] = flttocmplx( 0.0, 0.0 );
	Zero[3] = flttocmplx( 0.0, 0.0 );

	npole = 5;
	Pole[1] = flttocmplx( -5.0136607, 6.4615109 );
	Pole[2] = flttocmplx( -5.0136607, -6.4615109 );
	Pole[3] = flttocmplx( -8.2981509, 0.0 );
	Pole[4] = flttocmplx( -8.6940765, -7.1968661 );
	Pole[5] = flttocmplx( -8.6940765, 7.1968661 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

