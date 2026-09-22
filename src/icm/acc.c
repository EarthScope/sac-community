
#include "icm.h"

void /*FUNCTION*/
acc(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    double const_;
    complexd pole[2], zero[2];

    complexd *const Zero = &zero[0] - 1;

    /*   .....Acceleration Spectral Operator.....
     *
     *
     *   .....Set poles and zeros.....
     * */
    const_ = 1.0;
    npole = 0;
    nzero = 2;

    for (i = 1; i <= nzero; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
