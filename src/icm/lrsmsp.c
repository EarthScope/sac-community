
#include "icm.h"
#include "complex_sac.h"

void
lrsmsp(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    /*   .....LRSM SP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 3.9927709 e3   <*  AMP of 1.0 at FREQ of 1.0 Hz */
    complex double pole[5];
    complex double zero[3];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = 3.9927709e3;
    nzero = 3;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 5;
    Pole[1] = (-5.60893) + (7.40733 * I);
    Pole[2] = (-5.60893) + ((-7.40733) * I);
    Pole[3] = (-9.27502) + (0.0 * I);
    Pole[4] = (-28.24515) + (14.97041 * I);
    Pole[5] = (-28.24515) + ((-14.97041) * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

