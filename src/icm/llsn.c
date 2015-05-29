
#include "icm.h"
#include "complex_sac.h"

void
llsn(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    /*   .....LLSN - for an L-4 seismometer.....
     *     (poles and zeros due to P. Rodgers)
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = -3.57425 e15   <*  = -A * B */
    complex double pole[10];
    complex double zero[5];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = -3.57425e15;
    /*     nzero = 5              <* checked on 2/2/81 */
    nzero = 5;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 10;
    Pole[1] = (-5.026548) + (3.769911 * I);
    Pole[2] = (-5.026548) + ((-3.769911) * I);
    Pole[3] = (-0.5969026) + (0.0 * I);
    Pole[4] = (-0.5969026) + (0.0 * I);
    Pole[5] = (-276.460154) + (0.0 * I);
    Pole[6] = (-276.460154) + (0.0 * I);
    Pole[7] = (-376.99112) + (0.0 * I);
    Pole[8] = (-376.99112) + (0.0 * I);
    Pole[9] = (-571.76986) + (583.32194 * I);
    Pole[10] = (-571.76986) + ((-583.32194) * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

