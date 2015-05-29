
#include "icm.h"
#include "complex_sac.h"

void
ykasp(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    complex double pole[6];
    complex double zero[4];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;

    /*  .....YKA SP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 3.8605143 e5      <*   Amp of 1.0 at Freq of 1.0 Hz */
    const_ = 3.8605143e5;

    nzero = 4;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 6;
    Pole[1] = (-3.830) + (4.980 * I);
    Pole[2] = (-3.830) + ((-4.980) * I);
    Pole[3] = (-88.700) + (88.700 * I);
    Pole[4] = (-88.700) + ((-88.700) * I);
    Pole[5] = (-0.628) + (0.0 * I);
    Pole[6] = (-125.66) + (0.0 * I);

    /*   .....Compute transfer function.....
     * */
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

