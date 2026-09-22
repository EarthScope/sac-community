
#include "icm.h"


void /*FUNCTION*/
wwspbn(int nfreq, double delfrq, double xre[], double xim[])
{
    int npole, nzero;
    double const_;
    complexd pole[5], zero[3];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*  .....WWSSN SP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     * */
    const_ = 432.83395;

    nzero = 3;
    Zero[1] = dbltocmplx(0.0, 0.0);
    Zero[2] = dbltocmplx(0.0, 0.0);
    Zero[3] = dbltocmplx(0.0, 0.0);

    npole = 5;
    Pole[1] = dbltocmplx(-4.04094, 6.47935);
    Pole[2] = dbltocmplx(-4.04094, -6.47935);
    Pole[3] = dbltocmplx(-9.25238, 0.0);
    Pole[4] = dbltocmplx(-7.67430, 0.0);
    Pole[5] = dbltocmplx(-16.72981, 0.0);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
