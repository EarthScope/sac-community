
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ vel(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int npole, nzero;
	float const_;
	complexf pole[1], zero[1];

	complexf *const Zero = &zero[0] - 1;

	/*   .....VEL - velocity spectral operator.....
	 * */


	/*   .....Set poles and zeros.....
	 * */
	const_ = 1.0;
	nzero = 1;

	Zero[1] = flttocmplx( 0.0, 0.0 );

	npole = 0;

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

