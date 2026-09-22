
#include "icm.h"


void /*FUNCTION*/
ykasp(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    double const_;
    complexd pole[6], zero[4];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*  .....YKA SP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 3.8605143 e5      <*   Amp of 1.0 at Freq of 1.0 Hz */
    const_ = 3.8605143e5;

    nzero = 4;
    for (i = 1; i <= nzero; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }

    npole = 6;
    Pole[1] = dbltocmplx(-3.830, 4.980);
    Pole[2] = dbltocmplx(-3.830, -4.980);
    Pole[3] = dbltocmplx(-88.700, 88.700);
    Pole[4] = dbltocmplx(-88.700, -88.700);
    Pole[5] = dbltocmplx(-0.628, 0.0);
    Pole[6] = dbltocmplx(-125.66, 0.0);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
