
#include "icm.h"

void /*FUNCTION*/
ykalp(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    double const_;
    complexd pole[6], zero[4];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*  .....YKA LP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 0.28761294212      <*  AMP of 1.0 at FREQ of 0.05 Hz. */
    const_ = 0.28761294212;

    nzero = 4;
    for (i = 1; i <= nzero; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }

    npole = 6;
    Pole[1] = dbltocmplx(-0.2010, 0.2415);
    Pole[2] = dbltocmplx(-0.2010, -0.2415);
    Pole[3] = dbltocmplx(-0.134, 0.161);
    Pole[4] = dbltocmplx(-0.134, -0.161);
    Pole[5] = dbltocmplx(-0.628, 0.0);
    /*     pole(6) = cmplx ( -0.0134, 0.0 )              <*  KKN */
    Pole[6] = dbltocmplx(-0.0134, 0.0);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
