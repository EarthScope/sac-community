
#include "icm.h"
#include "complex_sac.h"

void
redkir(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    /*   .....RED KIRN - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 83.692405956 e0  <*  AMP of 1.0 at FREQ of 1.0 Hz */
    complex double pole[4];
    complex double zero[3];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = 83.692405956;
    nzero = 3;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 4;
    Pole[1] = (-0.12759) + (0.23031 * I);
    Pole[2] = (-0.12759) + ((-0.23031) * I);
    Pole[3] = (-0.29915) + (0.0 * I);
    Pole[4] = (-83.43929) + (0.0 * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

