
#include "icm.h"


void /*FUNCTION*/
ekalp6(nfreq, delfrq, xre, xim)
     int nfreq;
     double delfrq, xre[], xim[];
{
    int i, npole, nzero;
    double const_;
    complexd pole[9], zero[6];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....EKA LP6 - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *                             <*  AMP of 1.0 at FREQ of 0.05 Hz */
    const_ = 0.1084564;
    nzero = 6;
    for (i = 1; i <= 4; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }
    Zero[5] = dbltocmplx(-0.00524, 1.04720);
    Zero[6] = dbltocmplx(-0.00524, -1.04720);

    npole = 9;
    Pole[1] = dbltocmplx(-0.29323, 0.29915);
    Pole[2] = dbltocmplx(-0.29323, -0.29915);
    Pole[3] = dbltocmplx(-0.10996, 0.11218);
    Pole[4] = dbltocmplx(-0.10996, -0.11218);
    Pole[5] = dbltocmplx(-0.03140, 0.0);
    Pole[6] = dbltocmplx(-0.22000, 0.22400);
    Pole[7] = dbltocmplx(-0.22000, -0.22400);
    Pole[8] = dbltocmplx(-1.04720, 0.0);
    Pole[9] = dbltocmplx(-1.04720, 0.0);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
