
#include "icm.h"


void /*FUNCTION*/
lrsmlp(nfreq, delfrq, xre, xim)
     int nfreq;
     double delfrq, xre[], xim[];
{
    int i, npole, nzero;
    double const_;
    complexd pole[7], zero[4];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....LRSM LP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     * */
    const_ = 0.65871e-1;
    nzero = 4;

    for (i = 1; i <= nzero; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }

    npole = 7;
    Pole[1] = dbltocmplx(-0.19635, 0.24524);
    Pole[2] = dbltocmplx(-0.19635, -0.24524);
    Pole[3] = dbltocmplx(-0.20942, 0.0);
    Pole[4] = dbltocmplx(-0.20942, 0.0);
    Pole[5] = dbltocmplx(-0.00628, 0.0);
    Pole[6] = dbltocmplx(-0.17593, 0.17948);
    Pole[7] = dbltocmplx(-0.17593, -0.17948);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
