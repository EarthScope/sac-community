
#include "icm.h"
#include "complex_sac.h"

void
wwsp(int nfreq, double delfrq, double xre[], double xim[])
{
    int npole, nzero;
    float const_;
    complex double pole[5];
    complex double zero[3];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;

    /*  WWSSN short period seismometer.
     *  Ref:  Luh, P. C. (1977).  A scheme for expressing instrumental
     *  responses parametrically, BSSA, 67, 957-969.
     * */

    /*   .....Set poles and zeros.....
     * */
    const_ = 397.54767;
    nzero = 3;
    Zero[1] = 0.0 + (0.0 * I);
    Zero[2] = 0.0 + (0.0 * I);
    Zero[3] = 0.0 + (0.0 * I);

    npole = 5;
    Pole[1] = (-5.0136607) + (6.4615109 * I);
    Pole[2] = (-5.0136607) + ((-6.4615109) * I);
    Pole[3] = (-8.2981509) + (0.0 * I);
    Pole[4] = (-8.6940765) + ((-7.1968661) * I);
    Pole[5] = (-8.6940765) + (7.1968661 * I);
    /*   .....Compute transfer function.....
     * */
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

