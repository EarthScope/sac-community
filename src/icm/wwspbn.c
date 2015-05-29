
#include "icm.h"
#include "complex_sac.h"

void
wwspbn(int nfreq, double delfrq, double xre[], double xim[])
{
    int npole, nzero;
    float const_;
    complex double pole[5];
    complex double zero[3];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;

    /*  .....WWSSN SP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     * */
    const_ = 432.83395;
    nzero = 3;
    Zero[1] = 0.0 + (0.0 * I);
    Zero[2] = 0.0 + (0.0 * I);
    Zero[3] = 0.0 + (0.0 * I);

    npole = 5;
    Pole[1] = (-4.04094) + (6.47935 * I);
    Pole[2] = (-4.04094) + ((-6.47935) * I);
    Pole[3] = (-9.25238) + (0.0 * I);
    Pole[4] = (-7.67430) + (0.0 * I);
    Pole[5] = (-16.72981) + (0.0 * I);
    /*   .....Compute transfer function.....
     * */
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

