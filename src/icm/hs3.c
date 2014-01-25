
#include "icm.h"
#include "complex.h"

void /*FUNCTION*/ hs3(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int i;
	float delomg, omega;
	complexf cf1, cf2, cha, cha1, cha2, chb, chhp, chs3, cht, cs;
	static double twopi = 6.283185307179586;

	double *const Xim = &xim[0] - 1;
	double *const Xre = &xre[0] - 1;




	/*   .....S-750 Seismometer Response - output is in units of volts/nm....
	 * */



	delomg = twopi*delfrq;

	/*   .....Set poles and zeros.....
	 * */
	for( i = 1; i <= nfreq; i++ ){
		omega = (float)( i - 1 )*delomg;
		cs = flttocmplx( 0.0, omega );
		cht = cmplxdiv(flttocmplx(2.6e3,0.),(cmplxadd(cmplxadd(flttocmplx(1.11e5,0.),
		 cmplxmul(flttocmplx(33.3,0.),cs)),cmplxpow(cs,(double)2))));
		chhp = cmplxdiv(cmplxmul(flttocmplx(16.0,0.),cs),(cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(1.0e-4,0.),cs))));
		cha1 = cmplxdiv((cmplxadd(flttocmplx(21.0,0.),cmplxmul(flttocmplx(1.0e-4,0.),
		 cs))),(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(1.0e-4,0.),cs))));
		cha2 = cmplxneg(cmplxdiv(flttocmplx(2.0,0.),(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(2.0e-5,0.),
		 cs)))));
		cf1 = cmplxdiv((cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(1.7e-4,0.),cs))),
		 (cmplxadd(cmplxadd(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(1.038e-3,0.),
		 cs)),cmplxmul(flttocmplx(1.47e-7,0.),cmplxpow(cs,(double)2))),cmplxmul(flttocmplx(1.7e-12,0.),
		 cmplxpow(cs,(double)3)))));
		cf2 = cmplxmul(flttocmplx(3.0e-3,0.),cs);
		cha = cmplxmul(cmplxmul(cmplxmul(cht,chhp),cha1),cha2);
		chb = cmplxmul(cf1,cf2);
		chs3 = cmplxmul(cmplxmul(cmplxmul(cmplxmul(flttocmplx(100.0,0.),cs),(cmplxdiv(cha,
		 (cmplxsub(flttocmplx(1.0,0.),cmplxmul(cha,chb)))))),flttocmplx(1.0e-9,0.)),
		 cs);
		Xre[i] = cmplxtof( chs3 );
		Xim[i] = aimag( chs3 );
		}

	return;
} /* end of function */

