
#include "icm.h"


void /*FUNCTION*/
oldkir(nfreq, delfrq, xre, xim)
     int nfreq;
     double delfrq, xre[], xim[];
{
    int i, npole, nzero;
    double const_;
    complexd pole[4], zero[3];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....OLD KIRN - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 8.033295 e1   <*  AMP of 1.0 at FREQ of 1.0 Hz */
    const_ = 8.033295e1;
    nzero = 3;

    for (i = 1; i <= nzero; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }

    npole = 4;
    Pole[1] = dbltocmplx(-0.2140, 0.2300);
    Pole[2] = dbltocmplx(-0.2140, -0.2300);
    Pole[3] = dbltocmplx(-0.3150, 0.0);
    Pole[4] = dbltocmplx(-80.0000, 0.0);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
