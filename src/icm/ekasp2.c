
#include "icm.h"
#include "complex_sac.h"

void
ekasp2(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    /*   .....EKA SP2 - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *                                <*  AMP of 1.0 at FREQ of 1.0 Hz */
    complex double pole[4];
    complex double zero[4];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = 12.9083123695;
    nzero = 4;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 4;
    Pole[1] = (-3.7700) + (5.0200 * I);
    Pole[2] = (-3.7700) + ((-5.0200) * I);
    Pole[3] = (-67.2000) + (0.0 * I);
    Pole[4] = (-0.3300) + (0.0 * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

