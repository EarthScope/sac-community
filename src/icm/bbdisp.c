
#include "icm.h"
#include "complex_sac.h"

void
bbdisp(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    double const_;
    /*   .....BB DISP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *                           <*  AMP of 1.0 at FREQ of 1.0 Hz */
    double complex pole[7];
    double complex zero[3];
    double complex * const Pole = (&pole[0]) - 1;
    double complex * const Zero = (&zero[0]) - 1;
    const_ = 3.690549e5;
    nzero = 3;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 7;
    Pole[1] = (-0.2140) + (0.2300 * I);
    Pole[2] = (-0.2140) + ((-0.2300) * I);
    Pole[3] = (-6.4400) + (23.4000 * I);
    Pole[4] = (-6.4400) + ((-23.400) * I);
    Pole[5] = (-25.0000) + (0.0 * I);
    Pole[6] = (-25.0000) + (0.0 * I);
    Pole[7] = (-0.0555) + (0.0 * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

