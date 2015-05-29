
#include "icm.h"
#include "complex_sac.h"

void
wwlpbn(int nfreq, double delfrq, double xre[], double xim[])
{
    int npole, nzero;
    float const_;
    complex double pole[4];
    complex double zero[3];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;

    /*  .....WWSSN LP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 0.5985275       <*    AMP of 1.0 at FREQ of 0.05 Hz */
    const_ = 0.5985275;

    nzero = 3;
    Zero[1] = 0.0 + (0.0 * I);
    Zero[2] = 0.0 + (0.0 * I);
    Zero[3] = 0.0 + (0.0 * I);

    npole = 4;
    Pole[1] = (-0.257) + (0.3376 * I);
    Pole[2] = (-0.257) + ((-0.3376) * I);
    Pole[3] = (-0.06283) + (0.0 * I);
    Pole[4] = (-0.06283) + (0.0 * I);
    /*   .....Compute transfer function.....
     * */
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

