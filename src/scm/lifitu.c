
#include <math.h>

#include "scm.h"

void /*FUNCTION*/ lifitu(x, y, n, a, b, siga, sigb, sig, cc)
float x[], y[];
int n;
float *a, *b, *siga, *sigb, *sig, *cc;
{
	int i;
	float d, df, rn, sig2, siga2, sigb2, sumx, sumx2, sumxy, sumy, 
	 sumy2, xi, yi;

	float *const X = &x[0] - 1;
	float *const Y = &y[0] - 1;


	/*=====================================================================
	 * PURPOSE:  To apply a linear least squares fit to unevenly spaced data.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    x:       Independent variable data array. [ra] 
	 *    y:       Dependent variable data array. [ra]
	 *    n:       Number of points in data. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    a:       Slope of linear fit. [r]
	 *    b:       Y intercept of linear fit. [r]
	 *    siga:    Standard deviation of A. [r]
	 *    sigb:    Standard deviation of B. [r]
	 *    sig:     Standard deviation of data. [r]
	 *    cc:      Correlation coefficient between data and linear fit. [r]
	 *=====================================================================
	 * MODULE/LEVEL:  SERVICE/4
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    810528:  Original version from Steve Taylor.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Initialize accumulators. */
	rn = (float)( n );
	df = rn - 2.;
	sumx = 0.;
	sumy = 0.;
	sumxy = 0.;
	sumx2 = 0.;
	sumy2 = 0.;

	/* - Loop on each data point. */

	for( i = 1; i <= n; i++ ){
		xi = X[i];
		yi = Y[i];
		sumx = sumx + xi;
		sumy = sumy + yi;
		sumxy = sumxy + xi*yi;
		sumx2 = sumx2 + xi*xi;
		sumy2 = sumy2 + yi*yi;
		}

	/* - Calculate linear fit. */

	d = rn*sumx2 - sumx*sumx;
	*b = (sumx2*sumy - sumx*sumxy)/d;
	*a = (rn*sumxy - sumx*sumy)/d;

	/* - Estimate standard deviation in data. */

	sig2 = (sumy2 + rn**b**b + *a**a*sumx2 - 2.**b*sumy - 2.**a*sumxy + 
	 2.**b**a*sumx)/df;
	*sig = sqrt( sig2 );

	/* - Estimate errors in linear fit. */

	siga2 = rn*sig2/d;
	sigb2 = sig2*sumx2/d;
	*siga = sqrt( siga2 );
	*sigb = sqrt( sigb2 );

	/* - Calculate correlation coefficient between data and model. */

	*cc = (rn*sumxy - sumx*sumy)/sqrt( d*(rn*sumy2 - sumy*sumy) );
	*cc = fabs( *cc );

       
	return;
} /* end of function */

