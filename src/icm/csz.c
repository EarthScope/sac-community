
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ csz(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int npole, nzero;
	float const_;
	complexf pole[25], zero[9];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....RSTN CPO SP vertical seismometer transfer function.....
	 * */
	const_ = .30611e32*58000.0*1.0e-8;

	nzero = 9;
	Zero[1] = flttocmplx( -0.37700e3, 0.0 );
	Zero[2] = flttocmplx( 0.0, 0.0 );
	Zero[3] = flttocmplx( 0.0, 0.0 );
	Zero[4] = flttocmplx( 0.0, 0.0 );
	Zero[5] = flttocmplx( 0.0, 0.0 );
	Zero[6] = flttocmplx( 0.0, 0.0 );
	Zero[7] = flttocmplx( 0.0, 0.0 );
	Zero[8] = flttocmplx( 0.0, 0.0 );
	Zero[9] = flttocmplx( 0.0, 0.0 );

	npole = 25;
	Pole[1] = flttocmplx( -0.27287e3, -0.38935e3 );
	Pole[2] = flttocmplx( -0.27287e3, 0.38935e3 );
	Pole[3] = flttocmplx( -0.70460e2, -0.41884e2 );
	Pole[4] = flttocmplx( -0.70460e2, 0.41884e2 );
	Pole[5] = flttocmplx( -0.19294e3, 0.0 );
	Pole[6] = flttocmplx( -0.33303e3, 0.0 );
	Pole[7] = flttocmplx( -0.28643e-1, 0.0 );
	Pole[8] = flttocmplx( -0.75117e-1, -0.56925e-1 );
	Pole[9] = flttocmplx( -0.75117e-1, 0.56925e-1 );
	Pole[10] = flttocmplx( -0.62814e2, 0.0 );
	Pole[11] = flttocmplx( -0.62814e3, 0.0 );
	Pole[12] = flttocmplx( -0.36389e2, -0.18545e2 );
	Pole[13] = flttocmplx( -0.36389e2, 0.18545e2 );
	Pole[14] = flttocmplx( -0.28273e1, 0.0 );
	Pole[15] = flttocmplx( -0.18541e2, -0.36391e2 );
	Pole[16] = flttocmplx( -0.18541e2, 0.36391e2 );
	Pole[17] = flttocmplx( -0.28273e1, 0.0 );
	Pole[18] = flttocmplx( -0.40342e2, -0.63705e1 );
	Pole[19] = flttocmplx( -0.40342e2, 0.63705e1 );
	Pole[20] = flttocmplx( -0.28273e1, 0.0 );
	Pole[21] = flttocmplx( -0.63878e1, -0.40339e2 );
	Pole[22] = flttocmplx( -0.63878e1, 0.40339e2 );
	Pole[23] = flttocmplx( -0.28273e1, 0.0 );
	Pole[24] = flttocmplx( -0.28882e2, -0.28877e2 );
	Pole[25] = flttocmplx( -0.28882e2, 0.28877e2 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

