
#include <math.h>

#include "msg.h"
#include "scm.h"
#include "debug.h"

void
lifite(double x1, double dx, float *y, int n, float *a, float *b, float *siga,
       float *sigb, float *sig, float *cc) {
    int i;
    float d, df, rn, sig2, siga2, sigb2, xi, yi;
    double sumx, sumx2, sumxy, sumy, sumy2;

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
    rn = (float) (n);
    df = rn - 2.;
    sumx = 0.;
    sumy = 0.;
    sumxy = 0.;
    sumx2 = 0.;
    sumy2 = 0.;

    /* - Loop on each data point. */
    DEBUG("x1: %e\n", x1);
    for (i = 0; i < n; i++) {
        xi = x1 + dx * i;
        yi = y[i];
        sumx = sumx + xi;
        sumy = sumy + yi;
        sumxy = sumxy + xi * yi;
        sumx2 = sumx2 + xi * xi;
        sumy2 = sumy2 + yi * yi;
    }
    DEBUG("sumx %e sumy %e sumxy %e\n", sumx, sumy, sumxy);
    DEBUG("sumx2 %e sumy2: %e\n", sumx2, sumy2);
    /* - Calculate linear fit. */
    DEBUG("rn %e sumx2 %e %e\n", rn, sumx2*rn, sumx*sumx);
    d = rn * sumx2 - sumx * sumx;
    if(d < 0.0) {
        warning(1002, "variance of x, value < 0 : %e", d);
        outmsg();
    }
    *b = (sumx2 * sumy - sumx * sumxy) / d;
    *a = (rn * sumxy - sumx * sumy) / d;
    DEBUG("d %e a %e b %e %d\n", d, *a, *b, n);
    /* - Estimate standard deviation in data. */

    sig2 =
        (sumy2 + rn ** b ** b + *a ** a * sumx2 - 2. ** b * sumy -
         2. ** a * sumxy + 2. ** b ** a * sumx) / df;
    *sig = sqrt(sig2);

    /* - Estimate errors in linear fit. */

    siga2 = rn * sig2 / d;
    sigb2 = sig2 * sumx2 / d;
    DEBUG("siga2 %e sigb2 %e\n", siga2, sigb2);
    *siga = sqrt(siga2);
    *sigb = sqrt(sigb2);
    DEBUG("siga %e sigb %e\n", *siga, *sigb);
    /* - Calculate correlation coefficient between data and model. */

    *cc = (rn * sumxy - sumx * sumy) / sqrt(d * (rn * sumy2 - sumy * sumy));
    *cc = fabs(*cc);
    DEBUG("cc %e\n", *cc);
    return;

        /*=====================================================================
	 * MODIFICATION HISTORY:
	 *    810528:  Original version from Steve Taylor.
	 *===================================================================== */

}                               /* end of function */

void
lifite_(double *x1, double *dx, float *y, int *n, float *a, float *b,
        float *siga, float *sigb, float *sig, float *cc) {
    lifite(*x1, *dx, y, *n, a, b, siga, sigb, sig, cc);
}

void
lifite__(double *x1, double *dx, float *y, int *n, float *a, float *b,
         float *siga, float *sigb, float *sig, float *cc) {
    lifite(*x1, *dx, y, *n, a, b, siga, sigb, sig, cc);
}
