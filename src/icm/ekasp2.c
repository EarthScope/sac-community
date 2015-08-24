
#include "icm.h"


void /*FUNCTION*/
ekasp2(nfreq, delfrq, xre, xim)
     int nfreq;
     double delfrq, xre[], xim[];
{
    int i, npole, nzero;
    double const_;
    complexd pole[4], zero[4];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....EKA SP2 - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *                                <*  AMP of 1.0 at FREQ of 1.0 Hz */
    const_ = 12.9083123695;
    nzero = 4;
    for (i = 1; i <= nzero; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }

    npole = 4;
    Pole[1] = dbltocmplx(-3.7700, 5.0200);
    Pole[2] = dbltocmplx(-3.7700, -5.0200);
    Pole[3] = dbltocmplx(-67.2000, 0.0);
    Pole[4] = dbltocmplx(-0.3300, 0.0);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
