
#include <stdio.h>
#include <math.h>

#include "sam.h"


double /*FUNCTION*/ filtk(iopt, freq, yt)
int iopt;
double freq, yt;
{
	int i, i_, j, j_, k;
	float filtk_v;
	static double a[4], b[4], e1[3][4], e2[3][4], e3[3][4], e4[3][4], wct, 
	 wct2;
	static double pi = M_PI;
	static double c[8]={0.1950903e0,0.9807853e0,0.5555702e0,0.8314696e0,
	 0.8314696e0,0.5555702e0,0.9807853e0,0.1950903e0};

	double *const A = &a[0] - 1;
	double *const B = &b[0] - 1;
	double *const C = &c[0] - 1;



	/* . . .    Khronhite filter -- 8th order butterworth filter
	 *          using 4 cascaded 2nd order equations
	 *
	 * . . .    Linkage - Y = FILTK (IOPT, FREQ, YT)
	 *
	 * . . .    Arguments -
	 *            IOPT - 0 => initialize filter
	 *                   1 => filter radial data
	 *                   2 => filter tangential data
	 *                   3 => filter vertical data
	 *                   4 => extra
	 *            FREQ - the cut-off frequency for the filter
	 *            YT - if IOPT is zero, then this value is the sample time
	 *                 between the points
	 *               - if IOPT is nonzero, then the this value is the data point
	 *                 to be filtered
	 * */




	if( iopt != 0 )
		goto L_2000;

	/* . . .    INITIALIZE FILTER
	 * */
	for( i = 1; i <= 4; i++ ){
		i_ = i - 1;
		for( j = 1; j <= 3; j++ ){
			j_ = j - 1;
			e1[j_][i_] = 0.0;
			e2[j_][i_] = 0.0;
			e3[j_][i_] = 0.0;
			e4[j_][i_] = 0.0;
			}
		}

	wct = 2.0*pi*freq*yt;
	wct2 = wct*wct;

	for( i = 1; i <= 4; i++ ){
		j = 2*i - 1;
		k = 2*i;
		A[i] = 1.0 + 2.0*C[j]*wct + wct2*(C[j]*C[j] + C[k]*C[k]);
		B[i] = 2.0*(1.0 + C[j]*wct);
		}
	goto L_9000;

	/* . . .    FILTER
	 * */
L_2000:
	;
	e1[2][iopt - 1] = (wct2*yt + B[1]*e1[1][iopt - 1] - e1[0][iopt - 1])/
	 A[1];

	e2[2][iopt - 1] = (wct2*e1[2][iopt - 1] + B[2]*e2[1][iopt - 1] - 
	 e2[0][iopt - 1])/A[2];

	e3[2][iopt - 1] = (wct2*e2[2][iopt - 1] + B[3]*e3[1][iopt - 1] - 
	 e3[0][iopt - 1])/A[3];

	e4[2][iopt - 1] = (wct2*e3[2][iopt - 1] + B[4]*e4[1][iopt - 1] - 
	 e4[0][iopt - 1])/A[4];


	for( i = 1; i <= 2; i++ ){
		i_ = i - 1;
		e1[i_][iopt - 1] = e1[i_ + 1][iopt - 1];
		e2[i_][iopt - 1] = e2[i_ + 1][iopt - 1];
		e3[i_][iopt - 1] = e3[i_ + 1][iopt - 1];
		e4[i_][iopt - 1] = e4[i_ + 1][iopt - 1];
		}


L_9000:
	;
	filtk_v = e4[2][iopt - 1];
	return( filtk_v );
} /* end of function */

