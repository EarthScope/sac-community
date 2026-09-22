
#include "icm.h"


void /*FUNCTION*/
ptbllp(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    double const_;
    complexd pole[8], zero[5];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....PTBL LP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 2.554443      <*  AMP of 1.0 at FREQ of O.O5 Hz */
    const_ = 2.554443;
    nzero = 5;

    for (i = 1; i <= nzero; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }

    npole = 8;
    Pole[1] = dbltocmplx(-0.2930, 0.2950);
    Pole[2] = dbltocmplx(-0.2930, -0.2950);
    Pole[3] = dbltocmplx(-0.0256, 0.0439);
    Pole[4] = dbltocmplx(-0.0256, -0.0439);
    Pole[5] = dbltocmplx(-0.0417, 0.0417);
    Pole[6] = dbltocmplx(-0.0417, -0.0417);
    Pole[7] = dbltocmplx(-6.2800, 0.0);
    Pole[8] = dbltocmplx(-0.5700, 0.0);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
