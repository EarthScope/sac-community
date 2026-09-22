
#include "icm.h"

void /*FUNCTION*/
benbog(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    double const_;
    complexd pole[8], zero[4];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....BEN BOG - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *                             <*  AMP of 1.0 at FREQ of 1.0 Hz */
    const_ = 7.5111330e6;
    nzero = 4;
    for (i = 1; i <= nzero; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }

    npole = 8;
    Pole[1] = dbltocmplx(-10.539, 0.0);
    Pole[2] = dbltocmplx(-5.787, 7.407);
    Pole[3] = dbltocmplx(-5.787, -7.407);
    Pole[4] = dbltocmplx(-22.21, 22.21);
    Pole[5] = dbltocmplx(-22.21, -22.21);
    Pole[6] = dbltocmplx(-0.06283, 0.0);
    Pole[7] = dbltocmplx(-29.62, 29.62);
    Pole[8] = dbltocmplx(-29.62, -29.62);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
