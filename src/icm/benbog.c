
#include "icm.h"
#include "complex_sac.h"

void
benbog(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    /*   .....BEN BOG - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *                             <*  AMP of 1.0 at FREQ of 1.0 Hz */
    complex double pole[8];
    complex double zero[4];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = 7.5111330e6;
    nzero = 4;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 8;
    Pole[1] = (-10.539) + (0.0 * I);
    Pole[2] = (-5.787) + (7.407 * I);
    Pole[3] = (-5.787) + ((-7.407) * I);
    Pole[4] = (-22.21) + (22.21 * I);
    Pole[5] = (-22.21) + ((-22.21) * I);
    Pole[6] = (-0.06283) + (0.0 * I);
    Pole[7] = (-29.62) + (29.62 * I);
    Pole[8] = (-29.62) + ((-29.62) * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

