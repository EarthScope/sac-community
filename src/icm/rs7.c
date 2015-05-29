
#include <string.h>

#include "icm.h"
#include "complex_sac.h"

void
rs7(int nfreq, double delfrq, double xre[], double xim[], char *subtyp)
{
    int npole, nzero;
    float const_;
    /*   .....RSTN S-750 SP seismometer transfer function.....
     * */
    const_ = -0.10729e20 * 32000.0 * 1.0e-8;
    complex double pole[20];
    complex double zero[13];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    if (memcmp(subtyp, "ON7S.E  ", 8) == 0)
        const_ = -const_;

    nzero = 13;
    Zero[1] = (-0.37370e3) + (0.0 * I);
    Zero[2] = 0.0 + (0.0 * I);
    Zero[3] = 0.0 + (0.0 * I);
    Zero[4] = 0.0 + (0.0 * I);
    Zero[5] = 0.0 + (0.0 * I);
    Zero[6] = 0.0 + (0.0 * I);
    Zero[7] = (-0.11480e4) + (0.0 * I);
    Zero[8] = (-0.65050e4) + (0.0 * I);
    Zero[9] = (-0.78344e5) + (0.0 * I);
    Zero[10] = (-0.21120e6) + (0.0 * I);
    Zero[11] = 0.0 + (0.0 * I);
    Zero[12] = 0.0 + (0.0 * I);
    Zero[13] = 0.0 + (0.0 * I);
    npole = 20;
    Pole[1] = (-0.78828e5) + (0.0 * I);
    Pole[2] = (-0.49991e5) + (0.0 * I);
    Pole[3] = (-0.99000e4) + (0.0 * I);
    Pole[4] = (-0.67240e4) + (0.0 * I);
    Pole[5] = (-0.26310e3) + ((-0.40670e3) * I);
    Pole[6] = (-0.26310e3) + (0.40670e3 * I);
    Pole[7] = (-0.53020e3) + (0.0 * I);
    Pole[8] = (-0.62500e-1) + (0.0 * I);
    Pole[9] = (-0.99720) + ((-0.76530) * I);
    Pole[10] = (-0.99720) + (0.76530 * I);
    Pole[11] = (-0.12566e4) + (0.0 * I);
    Pole[12] = (-0.62814e3) + (0.0 * I);
    Pole[13] = (-0.28270e1) + (0.0 * I);
    Pole[14] = (-0.28270e1) + (0.0 * I);
    Pole[15] = (-0.28270e1) + (0.0 * I);
    Pole[16] = (-0.28270e1) + (0.0 * I);
    Pole[17] = (-0.86190e2) + ((-0.25840) * I);
    Pole[18] = (-0.86190e2) + (0.25840 * I);
    Pole[19] = (-0.62640e2) + ((-0.79080e2) * I);
    Pole[20] = (-0.62640e2) + (0.79080e2 * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

