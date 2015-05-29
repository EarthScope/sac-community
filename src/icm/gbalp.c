
#include "icm.h"
#include "complex_sac.h"

void
gbalp(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    /*   .....GBA LP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *                                <*  AMP of 1.0 at FREQ of 0.05 Hz */
    complex double pole[10];
    complex double zero[4];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = 0.100844452e-1;
    nzero = 4;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 10;
    Pole[1] = (-0.2140) + (0.2300 * I);
    Pole[2] = (-0.2140) + ((-0.2300) * I);
    Pole[3] = (-0.1340) + (0.1605 * I);
    Pole[4] = (-0.1340) + ((-0.1605) * I);
    Pole[5] = (-0.0312) + (0.0 * I);
    /*                                          <* KKN */
    Pole[6] = (-2.060) + (0.0 * I);
    Pole[7] = (-0.1670) + (0.2550 * I);
    Pole[8] = (-0.1670) + ((-0.2550) * I);
    Pole[9] = (-0.1670) + (0.2550 * I);
    Pole[10] = (-0.1670) + ((-0.2550) * I);
    /*   .....Compute transfer function.....
     * */
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

