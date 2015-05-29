
#include "icm.h"
#include "complex_sac.h"

void
hfslpwb(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    /*   .....HFS LPWB - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *                            <*  AMP of 1.0 at FREQ of 0.05 Hz */
    complex double pole[6];
    complex double zero[4];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = 0.1761249;
    nzero = 4;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 6;
    Pole[1] = (-0.1100) + (0.2380 * I);
    Pole[2] = (-0.1100) + ((-0.2380) * I);
    Pole[3] = (-0.1340) + (0.1605 * I);
    Pole[4] = (-0.1340) + ((-0.1605) * I);
    Pole[5] = (-0.0312) + (0.0 * I);
    Pole[6] = (-0.6450) + (0.0 * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

