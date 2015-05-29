
#include <string.h>

#include "icm.h"
#include "complex_sac.h"

void
rsk(int nfreq, double delfrq, double xre[], double xim[], char *subtyp)
{
    int npole, nzero;
    float const_;
    /*   .....RSTN KS-36000 SP seismometer transfer function.....
     * */
    const_ = 0.249e18 * 32000.0 * 1.0e-8;
    complex double pole[16];
    complex double zero[9];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    if (memcmp(subtyp, "ONKS.N  ", 8) == 0)
        const_ = 0.5 * const_;

    nzero = 9;
    Zero[1] = (-0.10067e4) + (0.0 * I);
    Zero[2] = (-0.40078e3) + (0.0 * I);
    Zero[3] = 0.0 + (0.0 * I);
    Zero[4] = 0.0 + (0.0 * I);
    Zero[5] = 0.0 + (0.0 * I);
    Zero[6] = 0.0 + (0.0 * I);
    Zero[7] = 0.0 + (0.0 * I);
    Zero[8] = 0.0 + (0.0 * I);
    Zero[9] = 0.0 + (0.0 * I);
    npole = 16;
    Pole[1] = (-0.12272e3) + ((-0.16843e3) * I);
    Pole[2] = (-0.12272e3) + (0.16843e3 * I);
    Pole[3] = (-0.13505e3) + (0.0 * I);
    Pole[4] = (-0.11287e4) + (0.0 * I);
    Pole[5] = (-0.99716) + ((-0.76548) * I);
    Pole[6] = (-0.99716) + (0.76548 * I);
    Pole[7] = (-0.12566e4) + (0.0 * I);
    Pole[8] = (-0.62814e3) + (0.0 * I);
    Pole[9] = (-0.28273e1) + (0.0 * I);
    Pole[10] = (-0.28273e1) + (0.0 * I);
    Pole[11] = (-0.28273e1) + (0.0 * I);
    Pole[12] = (-0.28273e1) + (0.0 * I);
    Pole[13] = (-0.86194e2) + ((-0.25840e2) * I);
    Pole[14] = (-0.86194e2) + (0.25840e2 * I);
    Pole[15] = (-0.62640e2) + ((-0.79078e2) * I);
    Pole[16] = (-0.62640e2) + (0.79078e2 * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

