
#include "icm.h"
#include "complex_sac.h"

void
bbvel(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    /*   .....BB VEL - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *                             <*  AMP of 1.0 at FREQ of 1.0 Hz */
    complex double pole[7];
    complex double zero[4];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = 5.873691e4;
    nzero = 4;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 7;
    Pole[1] = (-0.2140) + (0.2300 * I);
    Pole[2] = (-0.2140) + ((-0.2300) * I);
    Pole[3] = (-6.4400) + (23.4000 * I);
    Pole[4] = (-6.4400) + ((-23.4000) * I);
    Pole[5] = (-25.0000) + (0.0 * I);
    Pole[6] = (-25.0000) + (0.0 * I);
    Pole[7] = (-0.0555) + (0.0 * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

