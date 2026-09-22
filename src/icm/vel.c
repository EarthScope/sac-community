
#include "icm.h"


void /*FUNCTION*/
vel(int nfreq, double delfrq, double xre[], double xim[])
{
    int npole, nzero;
    double const_;
    complexd pole[1], zero[1];

    complexd *const Zero = &zero[0] - 1;

    /*   .....VEL - velocity spectral operator.....
     * */

    /*   .....Set poles and zeros.....
     * */
    const_ = 1.0;
    nzero = 1;

    Zero[1] = dbltocmplx(0.0, 0.0);

    npole = 0;

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
