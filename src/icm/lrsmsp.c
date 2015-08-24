
#include "icm.h"


void /*FUNCTION*/
lrsmsp(nfreq, delfrq, xre, xim)
     int nfreq;
     double delfrq, xre[], xim[];
{
    int i, npole, nzero;
    double const_;
    complexd pole[5], zero[3];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....LRSM SP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 3.9927709 e3   <*  AMP of 1.0 at FREQ of 1.0 Hz */
    const_ = 3.9927709e3;
    nzero = 3;

    for (i = 1; i <= nzero; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }

    npole = 5;
    Pole[1] = dbltocmplx(-5.60893, 7.40733);
    Pole[2] = dbltocmplx(-5.60893, -7.40733);
    Pole[3] = dbltocmplx(-9.27502, 0.0);
    Pole[4] = dbltocmplx(-28.24515, 14.97041);
    Pole[5] = dbltocmplx(-28.24515, -14.97041);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
