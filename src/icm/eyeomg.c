
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ eyeomg(nfreq, delfrq, xre, xim, nzer)
int nfreq;
double delfrq, xre[], xim[];
int nzer;
{
	int i, npole;
	float const_;
	complexf pole[30], zero[1];

	complexf *const Zero = &zero[0] - 1;

	/*   .....I - Omega.....
	 *
	 *
	 *   .....Set poles and zeros.....
	 * */
	const_ = 1.0;
	npole = 0;

	for( i = 1; i <= nzer; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzer, zero, npole, pole, xre, xim );

	return;
} /* end of function */

