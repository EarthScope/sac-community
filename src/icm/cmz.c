
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ cmz(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int npole, nzero;
	float const_;
	complexf pole[24], zero[7];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....RSTN CPO MP vertical seismometer transfer function.....
	 * */
	const_ = .13703e27*41500.0*1.0e-8;

	nzero = 7;
	Zero[1] = flttocmplx( -0.37700e3, 0.0 );
	Zero[2] = flttocmplx( 0.0, 0.0 );
	Zero[3] = flttocmplx( 0.0, 0.0 );
	Zero[4] = flttocmplx( 0.0, 0.0 );
	Zero[5] = flttocmplx( 0.0, 0.0 );
	Zero[6] = flttocmplx( 0.0, 0.0 );
	Zero[7] = flttocmplx( 0.0, 0.0 );

	npole = 24;
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
	Pole[12] = flttocmplx( -0.80650e1, -0.12880e1 );
	Pole[13] = flttocmplx( -0.80650e1, 0.12880e1 );
	Pole[14] = flttocmplx( -0.72780e1, -0.37070e1 );
	Pole[15] = flttocmplx( -0.72780e1, 0.37070e1 );
	Pole[16] = flttocmplx( -0.57740e1, -0.57770e1 );
	Pole[17] = flttocmplx( -0.57740e1, 0.57770e1 );
	Pole[18] = flttocmplx( -0.37090e1, -0.72770e1 );
	Pole[19] = flttocmplx( -0.37090e1, 0.72770e1 );
	Pole[20] = flttocmplx( -0.12770e1, -0.80670e1 );
	Pole[21] = flttocmplx( -0.12770e1, 0.80670e1 );
	Pole[22] = flttocmplx( -0.31450e-1, 0.0 );
	Pole[23] = flttocmplx( -0.31450e-1, 0.0 );
	Pole[24] = flttocmplx( -0.62814e2, 0.0 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

