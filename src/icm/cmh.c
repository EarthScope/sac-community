
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ cmh(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int npole, nzero;
	float const_;
	complexf pole[25], zero[8];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....RSTN CPO MP horizontal seismometer transfer function.....
	 * */
	const_ = .17443e28*30000.0*1.0e-8;

	nzero = 8;
	Zero[1] = flttocmplx( -0.10020e3, 0.0 );
	Zero[2] = flttocmplx( -0.60270e1, 0.0 );
	Zero[3] = flttocmplx( -0.18080e4, 0.0 );
	Zero[4] = flttocmplx( 0.0, 0.0 );
	Zero[5] = flttocmplx( 0.0, 0.0 );
	Zero[6] = flttocmplx( 0.0, 0.0 );
	Zero[7] = flttocmplx( 0.0, 0.0 );
	Zero[8] = flttocmplx( 0.0, 0.0 );

	npole = 25;
	Pole[1] = flttocmplx( -0.20140e5, 0.0 );
	Pole[2] = flttocmplx( -0.18066e4, 0.0 );
	Pole[3] = flttocmplx( -0.40256e3, 0.0 );
	Pole[4] = flttocmplx( -0.20216e3, 0.0 );
	Pole[5] = flttocmplx( -0.16698e2, -0.45746e2 );
	Pole[6] = flttocmplx( -0.16698e2, 0.45746e2 );
	Pole[7] = flttocmplx( -0.66753e-1, -0.58761e-1 );
	Pole[8] = flttocmplx( -0.66753e-1, 0.58761e-1 );
	Pole[9] = flttocmplx( -0.91893e2, 0.0 );
	Pole[10] = flttocmplx( -0.52330e1, 0.0 );
	Pole[11] = flttocmplx( -0.62814e2, 0.0 );
	Pole[12] = flttocmplx( -0.62814e3, 0.0 );
	Pole[13] = flttocmplx( -0.80650e1, -0.12880e1 );
	Pole[14] = flttocmplx( -0.80650e1, 0.12880e1 );
	Pole[15] = flttocmplx( -0.72780e1, -0.37070e1 );
	Pole[16] = flttocmplx( -0.72780e1, 0.37070e1 );
	Pole[17] = flttocmplx( -0.57740e1, -0.57770e1 );
	Pole[18] = flttocmplx( -0.57740e1, 0.57770e1 );
	Pole[19] = flttocmplx( -0.37090e1, -0.72770e1 );
	Pole[20] = flttocmplx( -0.37090e1, 0.72770e1 );
	Pole[21] = flttocmplx( -0.12770e1, -0.80670e1 );
	Pole[22] = flttocmplx( -0.12770e1, 0.80670e1 );
	Pole[23] = flttocmplx( -0.31450e-1, 0.0 );
	Pole[24] = flttocmplx( -0.31450e-1, 0.0 );
	Pole[25] = flttocmplx( -0.62814e2, 0.0 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

