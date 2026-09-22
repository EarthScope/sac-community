
#include "icm.h"


void /*FUNCTION*/
oldbb(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    double const_;
    complexd pole[5], zero[4];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....OLD BB - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 8.0336077 e1   <*  AMP of 1.0 at FREQ of 1.0 Hz */
    const_ = 8.0336077e1;
    nzero = 4;

    for (i = 1; i <= nzero; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }

    npole = 5;
    Pole[1] = dbltocmplx(-0.2140, 0.2300);
    Pole[2] = dbltocmplx(-0.2140, -0.2300);
    Pole[3] = dbltocmplx(-0.3150, 0.0);
    Pole[4] = dbltocmplx(-80.0000, 0.0);
    Pole[5] = dbltocmplx(-0.0555, 0.0);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
