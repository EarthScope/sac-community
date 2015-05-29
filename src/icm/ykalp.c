
#include "icm.h"
#include "complex_sac.h"

void
ykalp(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    complex double pole[6];
    complex double zero[4];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;

    /*  .....YKA LP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 0.28761294212      <*  AMP of 1.0 at FREQ of 0.05 Hz. */
    const_ = 0.28761294212;

    nzero = 4;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 6;
    Pole[1] = (-0.2010) + (0.2415 * I);
    Pole[2] = (-0.2010) + ((-0.2415) * I);
    Pole[3] = (-0.134) + (0.161 * I);
    Pole[4] = (-0.134) + ((-0.161) * I);
    Pole[5] = (-0.628) + (0.0 * I);
    /*     pole(6) = cmplx ( -0.0134, 0.0 )              <*  KKN */
    Pole[6] = (-0.0134) + (0.0 * I);

    /*   .....Compute transfer function.....
     * */
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

