
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ clh(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int npole, nzero;
	float const_;
	complexf pole[25], zero[8];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....RSTN CPO LP horizontal seismometer transfer function.....
	 * */
	const_ = .23249e19*7150.0*1.0e-8;

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
	Pole[13] = flttocmplx( -0.19582, -0.21692 );
	Pole[14] = flttocmplx( -0.19582, 0.21692 );
	Pole[15] = flttocmplx( -0.19582, -0.21692 );
	Pole[16] = flttocmplx( -0.19582, 0.21692 );
	Pole[17] = flttocmplx( -0.19725e1, -0.52866 );
	Pole[18] = flttocmplx( -0.19725e1, 0.52866 );
	Pole[19] = flttocmplx( -0.14441e1, -0.14438e1 );
	Pole[20] = flttocmplx( -0.14441e1, 0.14438e1 );
	Pole[21] = flttocmplx( -0.52856, -0.19725e1 );
	Pole[22] = flttocmplx( -0.52856, 0.19725e1 );
	Pole[23] = flttocmplx( -0.31417e-1, 0.0 );
	Pole[24] = flttocmplx( -0.31417e-1, 0.0 );
	Pole[25] = flttocmplx( -0.62832e1, 0.0 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

