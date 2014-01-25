
#include <math.h>

#include "icm.h"
#include "complex.h"
#include "co.h"

void /*FUNCTION*/ wiech(nfreq, delfrq, xre, xim)
int nfreq;
double delfrq, xre[], xim[];
{
	int npole, nzero;
	float const_, h, om0, rad, t0;
	complexf pole[2], zero[2];
	static double twopi = 6.283185307179586;

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....WIEC - for a Wiechert seismometer.....
	 * */



	/* MECHANICAL INSTRUMENT:  AMPLITUDE AND PHASE RESPONSES ARE COMPUTED
	 * FROM EQNS. (82), (98), AND (13) OF SOHON (1932), SEISMOMETRY, IN
	 * PART II OF INTRODUCTION TO THEORETICAL SEISMOLOGY, J. B. MACELWANE
	 * AND F. W. SOHON, JOHN WILEY AND SONS, NEW YORK.
	 * */
	t0 = 9.65;
	h = 0.403712752;
	const_ = 188.5;
	om0 = twopi/t0;
	nzero = 2;
	Zero[1] = flttocmplx(0.0,0.0);
	Zero[2] = flttocmplx(0.0,0.0);

	npole = 2;
	/*  ??????????????????????????????????????? */
	rad = sqrt( 1.0 - powi(h,2) );
	Pole[1] = flttocmplx( -om0*h, om0*rad );
	Pole[2] = flttocmplx( -om0*h, -om0*rad );
	/*  ???????????????????????????????????????
	 *
	 *   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

	return;
} /* end of function */

