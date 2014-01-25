
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ gsref(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i, npole, nzero;
	float const_;
	complexf pole[11], zero[5];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....USGS Refraction Seismometer.....
	 *
	 *
	 *   .....Set poles and zeros (due to J. Zucca).....
	 * */
	const_ = 276.46*276.46*283.177*293.349*293.349*330.873*330.873;
	nzero = 5;

	for( i = 1; i <= nzero; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}

	npole = 11;
	Pole[1] = cmplxneg(flttocmplx( 10.0531, 7.5398 ));
	Pole[2] = cmplxneg(flttocmplx( 10.0531, -7.5398 ));
	Pole[3] = cmplxneg(flttocmplx( 0.5969, 0.0 ));
	Pole[4] = cmplxneg(flttocmplx( 0.5969, 0.0 ));
	Pole[5] = cmplxneg(flttocmplx( 276.4602, 0.0 ));
	Pole[6] = cmplxneg(flttocmplx( 276.4602, 0.0 ));
	Pole[7] = cmplxneg(flttocmplx( 283.1769, 0.0 ));
	Pole[8] = cmplxneg(flttocmplx( 260.2009, 135.4598 ));
	Pole[9] = cmplxneg(flttocmplx( 260.2009, -135.4598 ));
	Pole[10] = cmplxneg(flttocmplx( 180.6564, 277.2001 ));
	Pole[11] = cmplxneg(flttocmplx( 180.6564, -277.2001 ));

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

