
#include "icm.h"
#include "complex_sac.h"

void
ptbllp(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    /*   .....PTBL LP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 2.554443      <*  AMP of 1.0 at FREQ of O.O5 Hz */
    complex double pole[8];
    complex double zero[5];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = 2.554443;
    nzero = 5;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 8;
    Pole[1] = (-0.2930) + (0.2950 * I);
    Pole[2] = (-0.2930) + ((-0.2950) * I);
    Pole[3] = (-0.0256) + (0.0439 * I);
    Pole[4] = (-0.0256) + ((-0.0439) * I);
    Pole[5] = (-0.0417) + (0.0417 * I);
    Pole[6] = (-0.0417) + ((-0.0417) * I);
    Pole[7] = (-6.2800) + (0.0 * I);
    Pole[8] = (-0.5700) + (0.0 * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

