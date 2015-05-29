
#include "icm.h"
#include "complex_sac.h"

void
ekalp6(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    /*   .....EKA LP6 - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *                             <*  AMP of 1.0 at FREQ of 0.05 Hz */
    complex double pole[9];
    complex double zero[6];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = 0.1084564;
    nzero = 6;
    for (i = 1; i <= 4; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    Zero[5] = (-0.00524) + (1.04720 * I);
    Zero[6] = (-0.00524) + ((-1.04720) * I);
    npole = 9;
    Pole[1] = (-0.29323) + (0.29915 * I);
    Pole[2] = (-0.29323) + ((-0.29915) * I);
    Pole[3] = (-0.10996) + (0.11218 * I);
    Pole[4] = (-0.10996) + ((-0.11218) * I);
    Pole[5] = (-0.03140) + (0.0 * I);
    Pole[6] = (-0.22000) + (0.22400 * I);
    Pole[7] = (-0.22000) + ((-0.22400) * I);
    Pole[8] = (-1.04720) + (0.0 * I);
    Pole[9] = (-1.04720) + (0.0 * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

