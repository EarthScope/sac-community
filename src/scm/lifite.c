
#include <math.h>

#include "scm.h"

void /*FUNCTION*/ lifite(x1, dx, y, n, a, b, siga, sigb, sig, cc)
double x1, dx;
float y[];
int n;
float *a, *b, *siga, *sigb, *sig, *cc;
{
	int i;
	float d, df, rn, sig2, siga2, sigb2, sumx, sumx2, sumxy, sumy, 
	 sumy2, xi, yi;

	float *const Y = &y[0] - 1;


	/*=====================================================================
	 * PURPOSE:  To apply a linear least squares fit to evenly spaced data.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    X1:      Beginning value of independent variable.
	 *    DX:      Increment between values of independent variable.
	 *    Y:       Dependent variable data array.
	 *    N:       Number of points in data.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    A:       Slope of linear fit.
	 *    B:       Y intercept of linear fit.
	 *    SIGA:    Standard deviation of A.
	 *    SIGB:    Standard deviation of B.
	 *    SIG:     Standard deviation of data.
	 *    CC:      Correlation coefficient between data and linear fit.
	 *=====================================================================
	 * MODULE/LEVEL:  SERVICE/4
	 *=====================================================================
	 * LOCAL VARIABLES:
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

	xi = x1;
	for( i = 1; i <= n; i++ ){
		yi = Y[i];
    xi = x1 + (dx * (i-1));
		sumx = sumx + xi;
		sumy = sumy + Y[i];
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

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    810528:  Original version from Steve Taylor.
	 *===================================================================== */

} /* end of function */

