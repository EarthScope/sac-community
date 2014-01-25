
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ noresshf(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i;
	float delomg, omega;
	complexf chhf, chhp, chia, chif, chlp, chpa, chsi, cs, temp1, 
	 temp2, temp3;
	static double twopi = 6.283185307179586;

	double *const Xim = &xim[0] - 1;
	double *const Xre = &xre[0] - 1;




	/*   .....NORESS high frequency element (Durham, 6 March 1986).....
	 *          output in volts/nm
	 *          A. Smith, 6 May 1986
	 * */


	delomg = twopi*delfrq;

	for( i = 1; i <= nfreq; i++ ){
		omega = (float)( i - 1 )*delomg;
		cs = flttocmplx( 0.0, omega );
		chsi = cmplxdiv(cmplxmul(flttocmplx(55.73,0.),cmplxpow(cs,(double)2)),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(0.2387,0.),cs)),cmplxmul(flttocmplx(0.02533,0.),cmplxpow(cs,(double)2)))));
		chif = cmplxdiv(flttocmplx(0.9174,0.),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(8.165e-4,0.),
		 cs)),cmplxmul(flttocmplx(7.339e-7,0.),cmplxpow(cs,(double)2)))));
		chpa = cmplxdiv(cmplxmul(flttocmplx(5.544,0.),cs),(cmplxmul((cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(1.559e-3,0.),cs))),(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.1598,0.),
		 cs))))));
		chia = cmplxdiv(cmplxmul(flttocmplx(0.3186,0.),cs),(cmplxmul((cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(1.593,0.),cs))),(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(2.496e-3,0.),
		 cs))))));

		/* Use of temporary variables below due to bug in MASSCOMP compiler (870528) */

		temp1 = cmplxmul(cs,cs);
		temp2 = cmplxmul(flttocmplx(0.0124,0.),temp1);
		temp3 = (cmplxmul((cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.0500,0.),
		 cs))),(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.124,0.),cs)))));
		chhp = cmplxdiv(temp2,temp3);

		chlp = cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(flttocmplx(1.0,0.),cmplxpow((cmplxmul(flttocmplx(2.894e-3,0.),
		 cs)),(double)36))));

		chhf = cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(chsi,chif),
		 chpa),chia),chhp),chlp),flttocmplx(1.0e-9,0.)),cs);

		Xre[i] = cmplxtof( chhf );
		Xim[i] = aimag( chhf );
		}
	return;
} /* end of function */

