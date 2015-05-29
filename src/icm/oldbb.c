
#include "icm.h"
#include "complex_sac.h"

void
oldbb(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    /*   .....OLD BB - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 8.0336077 e1   <*  AMP of 1.0 at FREQ of 1.0 Hz */
    complex double pole[5];
    complex double zero[4];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = 8.0336077e1;
    nzero = 4;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 5;
    Pole[1] = (-0.2140) + (0.2300 * I);
    Pole[2] = (-0.2140) + ((-0.2300) * I);
    Pole[3] = (-0.3150) + (0.0 * I);
    Pole[4] = (-80.0000) + (0.0 * I);
    Pole[5] = (-0.0555) + (0.0 * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

