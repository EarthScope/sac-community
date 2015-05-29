
#include "icm.h"
#include "complex_sac.h"

void
gbasp(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    /*   .....GBA SP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *                             <*  AMP of 1.0 at FREQ of 1.6 Hz */
    complex double pole[4];
    complex double zero[3];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = 2.5597471e2;
    nzero = 3;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 4;
    Pole[1] = (-4.02) + (4.82 * I);
    Pole[2] = (-4.02) + ((-4.82) * I);
    Pole[3] = (-40.2) + (30.2 * I);
    Pole[4] = (-40.2) + ((-30.2) * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

