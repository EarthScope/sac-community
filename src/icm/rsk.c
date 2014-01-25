
#include <string.h>

#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ rsk(nfreq, delfrq, xre, xim, subtyp)
int nfreq;
double delfrq, xre[], xim[];
char *subtyp;
{
	int npole, nzero;
	float const_;
	complexf pole[16], zero[9];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....RSTN KS-36000 SP seismometer transfer function.....
	 * */
	const_ = 0.249e18*32000.0*1.0e-8;
	if( memcmp(subtyp,"ONKS.N  ",8) == 0 )
		const_ = 0.5*const_;

	nzero = 9;
	Zero[1] = flttocmplx( -0.10067e4, 0.0 );
	Zero[2] = flttocmplx( -0.40078e3, 0.0 );
	Zero[3] = flttocmplx( 0.0, 0.0 );
	Zero[4] = flttocmplx( 0.0, 0.0 );
	Zero[5] = flttocmplx( 0.0, 0.0 );
	Zero[6] = flttocmplx( 0.0, 0.0 );
	Zero[7] = flttocmplx( 0.0, 0.0 );
	Zero[8] = flttocmplx( 0.0, 0.0 );
	Zero[9] = flttocmplx( 0.0, 0.0 );

	npole = 16;
	Pole[1] = flttocmplx( -0.12272e3, -0.16843e3 );
	Pole[2] = flttocmplx( -0.12272e3, 0.16843e3 );
	Pole[3] = flttocmplx( -0.13505e3, 0.0 );
	Pole[4] = flttocmplx( -0.11287e4, 0.0 );
	Pole[5] = flttocmplx( -0.99716, -0.76548 );
	Pole[6] = flttocmplx( -0.99716, 0.76548 );
	Pole[7] = flttocmplx( -0.12566e4, 0.0 );
	Pole[8] = flttocmplx( -0.62814e3, 0.0 );
	Pole[9] = flttocmplx( -0.28273e1, 0.0 );
	Pole[10] = flttocmplx( -0.28273e1, 0.0 );
	Pole[11] = flttocmplx( -0.28273e1, 0.0 );
	Pole[12] = flttocmplx( -0.28273e1, 0.0 );
	Pole[13] = flttocmplx( -0.86194e2, -0.25840e2 );
	Pole[14] = flttocmplx( -0.86194e2, 0.25840e2 );
	Pole[15] = flttocmplx( -0.62640e2, -0.79078e2 );
	Pole[16] = flttocmplx( -0.62640e2, 0.79078e2 );

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

