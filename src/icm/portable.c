
#include <math.h>

#include "icm.h"
#include "co.h"
#include "complex.h"

void /*FUNCTION*/ portable(nfreq, delfrq, xre, xim, freepd, damp, 
	 crfrq)
int nfreq;
double delfrq, xre[], xim[], freepd, damp, crfrq;
{
	int i, j, npole, nzero;
	float anorm, asqrd, astest, cf, comp, const_, dc, delomg, discrm, 
	 fac, fp, omega, s1i, s1r, s2i, s2r, sfil1i, sfil1r, sfil2i, sfil2r, 
	 sqr202, ti, ti0, tid, tin, tr, tr0, trd, trn, xxim, xxre;
	complexf pole[4], zero[2];
	static double twopi = 6.283185307179586;

	complexf *const Pole = &pole[0] - 1;
	double *const Xim = &xim[0] - 1;
	double *const Xre = &xre[0] - 1;
	complexf *const Zero = &zero[0] - 1;




	/*   .....PORTABLE - for portable seismometer-PDR2 response.....
	 *             (poles and zeros due to H. Patton)
	 * */



	sqr202 = 0.707106781;
	delomg = twopi*delfrq;
	/*     fp = twopi / freepd       <*   (Radians) */
	fp = twopi/freepd;
	dc = damp;
	/*     cf = twopi * crfrq        <*   Corner Frequency ( Radians ) */
	cf = twopi*crfrq;

	/*   .....Two pole Butterworth filter.....
	 * */
	comp = sqr202*cf;
	/*     const = comp * comp       <*   numerator */
	const_ = comp*comp;
	sfil1r = -comp;
	sfil1i = comp;
	sfil2r = -comp;
	sfil2i = -comp;

	/*   Seismometer
	 *    Allow for variable seismometer free period and damping constant.
	 *    Numerator: S*S ( for velocitygram ).
	 *    Poles are roots of denominator:
	 *                 2                 2
	 *              (S) + 2DC(FP)S + (FP) = 0.0
	 *         Where:
	 *                 S = i * omega
	 *                DC = damping constant.
	 *                FP = 2 Pi * ( free period in sec. )    <*   (radians)
	 * */
	discrm = dc*dc - 1.0;
	if( discrm > 0.0 ){

		/*              .....Overdamped.....
		 * */
		discrm = sqrt( discrm );
		s1r = fp*(-dc + discrm);
		s1i = 0.0;
		s2r = fp*(-dc - discrm);
		s2i = 0.0;

		}
	else{
		discrm = fabs( discrm );
		discrm = sqrt( discrm );
		s1r = -dc*fp;
		s1i = fp*discrm;
		s2r = s1r;
		s2i = -s1i;
		}

	/*   .....computing velocity response.....
	 * */
	nzero = 2;
	/*     zero(1) = cmplx ( 0.0, 0.0 )              <*  Seismometer. */
	Zero[1] = flttocmplx( 0.0, 0.0 );
	Zero[2] = flttocmplx( 0.0, 0.0 );

	npole = 4;
	/*     pole(1) = cmplx ( sfil1r, sfil1i )        <*  Filter */
	Pole[1] = flttocmplx( sfil1r, sfil1i );
	/*     pole(2) = cmplx ( sfil2r, sfil2i )        <*  Filter */
	Pole[2] = flttocmplx( sfil2r, sfil2i );
	/*     pole(3) = cmplx ( s1r, s1i )              <*  Seismometer */
	Pole[3] = flttocmplx( s1r, s1i );
	/*     pole(4) = cmplx ( s2r, s2i )              <*  Seismometer */
	Pole[4] = flttocmplx( s2r, s2i );

	asqrd = 0.0;
	for( j = 1; j <= nfreq; j++ ){
		omega = delomg*(float)( j - 1 );
		trn = 1.0e0;
		tin = 0.0e0;

		for( i = 1; i <= nzero; i++ ){
			tr = -cmplxtof( Zero[i] );
			ti = omega - aimag( Zero[i] );
			tr0 = trn*tr - tin*ti;
			ti0 = trn*ti + tin*tr;
			trn = tr0;
			tin = ti0;
			}

		trd = 1.0e0;
		tid = 0.0e0;
		for( i = 1; i <= npole; i++ ){
			tr = -cmplxtof( Pole[i] );
			ti = omega - aimag( Pole[i] );
			tr0 = trd*tr - tid*ti;
			ti0 = trd*ti + tid*tr;
			trd = tr0;
			tid = ti0;
			}
		fac = (double)( const_ )/(powi(trd,2) + powi(tid,2));
		Xre[j] = fac*(trn*trd + tin*tid);
		Xim[j] = fac*(trd*tin - trn*tid);
		astest = powi(Xre[j],2) + powi(Xim[j],2);
		if( astest > asqrd )
			asqrd = astest;
		}

	/*   .....Normalization such that velocity transfer function has
	 *      maximum equal to unity i. e. displacement transfer function
	 *      has gain equal to 2*PI*FMAX where FMAX is maximum point on
	 *      velocity transfer function.....
	 *
	 *   .....Also transform velocity response to displacement response
	 *      - multiply by I * OMEGA.....
	 * */
	anorm = 1.0/sqrt( asqrd );
	for( j = 1; j <= nfreq; j++ ){
		omega = delomg*(float)( j - 1 );
		xxre = -Xim[j]*(double)( omega*anorm );
		xxim = Xre[j]*(double)( omega*anorm );
		Xre[j] = xxre;
		Xim[j] = xxim;
		}

	return;
} /* end of function */

