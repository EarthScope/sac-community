
#include "icm.h"
#include "complex_sac.h"

void
wabn(int nfreq, double delfrq, double xre[], double xim[])
{
    int npole, nzero;
    float const_;
    complex double pole[2];
    complex double zero[2];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;

    /*   .....WABN - WOOD-ANDERSON: Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 2.077594       <*    AMP of 1.0 at FREQ of 1.0 Hz */
    const_ = 2.077594;
    nzero = 2;
    Zero[1] = 0.0 + (0.0 * I);
    Zero[2] = 0.0 + (0.0 * I);

    npole = 2;
    Pole[1] = (-6.28318) + (4.71239 * I);
    Pole[2] = (-6.28318) + ((-4.71239) * I);

    /*   .....Compute transfer function.....
     * */
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

