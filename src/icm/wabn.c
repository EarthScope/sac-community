
#include "icm.h"


void /*FUNCTION*/
wabn(int nfreq, double delfrq, double xre[], double xim[])
{
    int npole, nzero;
    double const_;
    complexd pole[2], zero[2];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....WABN - WOOD-ANDERSON: Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 2.077594       <*    AMP of 1.0 at FREQ of 1.0 Hz */
    const_ = 2.077594;
    nzero = 2;
    Zero[1] = dbltocmplx(0.0, 0.0);
    Zero[2] = dbltocmplx(0.0, 0.0);

    npole = 2;
    Pole[1] = dbltocmplx(-6.28318, 4.71239);
    Pole[2] = dbltocmplx(-6.28318, -4.71239);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
