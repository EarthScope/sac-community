
#include "icm.h"
#include "complex_sac.h"

void
lrsmlp(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    /*   .....LRSM LP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     * */
    complex double pole[7];
    complex double zero[4];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = 0.65871e-1;
    nzero = 4;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 7;
    Pole[1] = (-0.19635) + (0.24524 * I);
    Pole[2] = (-0.19635) + ((-0.24524) * I);
    Pole[3] = (-0.20942) + (0.0 * I);
    Pole[4] = (-0.20942) + (0.0 * I);
    Pole[5] = (-0.00628) + (0.0 * I);
    Pole[6] = (-0.17593) + (0.17948 * I);
    Pole[7] = (-0.17593) + ((-0.17948) * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

