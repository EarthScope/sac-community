
#include "icm.h"
#include "co.h"
#include "complex.h"


#include "msg.h"

void /*FUNCTION*/ elmag(nfreq, delfrq, xre, xim, freepd, mag, nerr)
int nfreq;
double delfrq, xre[], xim[], freepd, mag;
int *nerr;
{
	int i, nmag;
	float const_, delomg, h1, h2, om1, omega, p, sigsq, t1, t2;
	complexf cs, ct, ctd;
	static double twopi = 6.283185307179586;

	double *const Xim = &xim[0] - 1;
	double *const Xre = &xre[0] - 1;


	/* for a WWSSN Elecromagnetic Instrument
	 *
	 * */



	/* ELECTROMAGNETIC INSTRUMENT:  AMPLITUDE AND PHASE RESPONSES ARE
	 * COMPUTED FROM EQN. (28) AND (29) --- SEE COMMENT PRECEDING STATEMENT
	 * NUMBER D460 --- OF HAGIWARA (1958) , BULL. EARTHQ. RES. INST., TOKYO
	 * UNIV., VOL. 36, P.139-164.
	 *
	 * READ T1, T2, H1, H2, AND SIGSQ.
	 *   T1 IS THE PENDULUM PERIOD, T2 IS THE GALVANOMETER PERIOD.
	 *   H1 IS THE PENDULUM DAMPING FACTOR AND H2 IS THE GALVANOMETER
	 *    DAMPING FACTOR.
	 *   SIGSQ IS THE INSTRUMENT COUPLING FACTOR AS DEFINED BY HAGIWARA.
	 *   ACCORDING TO CHANDRA (1970), BSSA, 60, 539-563,
	 *       FOR T1=15, T2=100, H1=0.93, H2=1.0
	 *
	 *         PEAK MAGNIFICATION      SIGSQ
	 *
	 *                 375             0.003
	 *                 750             0.013
	 *                1500             0.047
	 *                3000             0.204
	 *                6000             0.805
	 *
	 *       FOR T1=30, T2=100, H1=1.50, H2=1.0
	 *
	 *         PEAK MAGNIFICATION      SIGSQ
	 *
	 *                 375             0.003
	 *                 750             0.012
	 *                1500             0.044
	 *                3000             0.195
	 *                6000             0.767
	 * */
	*nerr = 0;
	delomg = twopi*delfrq;

	nmag = (int)( mag + 0.01 );
	if( freepd == 15.0 ){

		t1 = 15.0;
		t2 = 100.0;
		h1 = .93;
		h2 = 1.0;
		if( nmag == 375 ){
			const_ = 712.48076;
			sigsq = 0.003;
			}
		else if( nmag == 750 ){
			const_ = 1423.9605;
			sigsq = 0.013;
			}
		else if( nmag == 1500 ){
			const_ = 2840.9091;
			sigsq = 0.047;
			}
		else if( nmag == 3000 ){
			const_ = 2708.3153;
			sigsq = 0.204;
			}
		else if( nmag == 6000 ){
			const_ = 10114.803;
			sigsq = 0.805;
			}
		else{
			*nerr = 2112;
			setmsg( "ERROR", *nerr );
			goto L_8888;
			}

		}
	else if( freepd == 30.0 ){

		t1 = 30.0;
		t2 = 100.0;
		h1 = 1.5;
		h2 = 1.0;
		if( nmag == 375 ){
			const_ = 1202.5398;
			sigsq = 0.003;
			}
		else if( nmag == 750 ){
			const_ = 2402.1523;
			sigsq = 0.012;
			}
		else if( nmag == 1500 ){
			const_ = 4781.9434;
			sigsq = 0.044;
			}
		else if( nmag == 3000 ){
			const_ = 9272.1372;
			sigsq = 0.195;
			}
		else if( nmag == 6000 ){
			const_ = 10703.391;
			sigsq = 0.767;
			}
		else{
			*nerr = 2112;
			setmsg( "ERROR", *nerr );
			goto L_8888;
			}

		}
	else{

		*nerr = 2112;
		setmsg( "ERROR", *nerr );
		goto L_8888;

		}

	p = t1/t2;
	om1 = twopi/t1;
	for( i = 1; i <= nfreq; i++ ){
		omega = (float)( i - 1 )*delomg;
		cs = flttocmplx( 0.0, omega );
		ctd = cmplxadd(cmplxpow(cs,(double)4),cmplxmul(flttocmplx(2.0*om1*(h1 + 
		 p*h2),0.),cmplxpow(cs,(double)3)));
		ctd = cmplxadd(ctd,cmplxmul(flttocmplx(((1.0 + powi(p,2)) + 4.0*h1*h2*
		 p*(1.0 - sigsq))*powi(om1,2),0.),cmplxpow(cs,(double)2)));
		ctd = cmplxadd(ctd,cmplxmul(flttocmplx(2.0*(p*h1 + h2)*p*powi(om1,3),0.),
		 cs));
		ctd = cmplxadd(ctd,flttocmplx(powi(p,2)*powi(om1,4),0.));
		ct = cmplxdiv(cmplxmul(flttocmplx(om1,0.),cmplxpow(cs,(double)3)),ctd);

		/* THE PHASE RESPONSE IS DEFINED AS NEGATIVE THE PHASE RESPONSE OF
		 * HAGIWARA IN ORDER TO YIELD UPWARD FIRST MOTION ON THE SEISMOGRAM TRACE
		 * FOR AN IMPULSE OF GROUND DISPLACEMENT IN THE POSITIVE PHI DIRECTION.
		 * */
		ct = cmplxmul(flttocmplx(const_,0.),ct);
		Xre[i] = cmplxtof( ct );
		Xim[i] = aimag( ct );
		}

L_8888:
	return;

} /* end of function */

