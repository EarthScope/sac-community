
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ acc(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i, npole, nzero;
	float const_;
	complexf pole[2], zero[2];

	complexf *const Zero = &zero[0] - 1;

	/*   .....Acceleration Spectral Operator.....
	 *
	 *
	 *   .....Set poles and zeros.....
	 * */
	const_ = 1.0;
	npole = 0;
	nzero = 2;

	for( i = 1; i <= nzero; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

