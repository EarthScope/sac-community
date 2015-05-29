
#include "icm.h"
#include "complex_sac.h"

void
oldkir(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    /*   .....OLD KIRN - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 8.033295 e1   <*  AMP of 1.0 at FREQ of 1.0 Hz */
    complex double pole[4];
    complex double zero[3];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = 8.033295e1;
    nzero = 3;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 4;
    Pole[1] = (-0.2140) + (0.2300 * I);
    Pole[2] = (-0.2140) + ((-0.2300) * I);
    Pole[3] = (-0.3150) + (0.0 * I);
    Pole[4] = (-80.0000) + (0.0 * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

