
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ dwwssn(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i, npole, nzero;
	float const_;
	complexf pole[11], zero[5];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....DWWSSN - For a digital WWSSN system response.....
	 *            ( poles and zeros due to H. Patton )
	 *
	 *     digital WWSSN instrumental response.....
	 *     default response computed assuming poles and zeros for
	 *     stations LON and JAS supplied on day tapes for day 
	 *     301, 1983
	 * */


	/*   .....Set poles and zeros.....
	 * */
	const_ = 0.0243*500.;
	nzero = 5;
	for( i = 1; i <= nzero; i++ ){
		Zero[i] = flttocmplx( 0.0, 0.0 );
		}

	npole = 11;
	Pole[1] = flttocmplx( -0.369, .199 );
	Pole[2] = flttocmplx( -0.369, -.199 );
	Pole[3] = flttocmplx( -0.628, 0.0 );
	Pole[4] = flttocmplx( -0.0209, 0.0 );
	Pole[5] = flttocmplx( -0.0209, 0.0 );
	for( i = 6; i <= npole; i++ ){
		Pole[i] = flttocmplx( -.273, 0.0 );
		}

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

