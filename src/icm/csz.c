
#include "icm.h"
#include "complex_sac.h"

void
csz(int nfreq, double delfrq, double xre[], double xim[])
{
    int npole, nzero;
    float const_;
    /*   .....RSTN CPO SP vertical seismometer transfer function.....
     * */
    complex double pole[25];
    complex double zero[9];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = (.30611e32 * 58000.0) * 1.0e-8;
    nzero = 9;
    Zero[1] = (-0.37700e3) + (0.0 * I);
    Zero[2] = 0.0 + (0.0 * I);
    Zero[3] = 0.0 + (0.0 * I);
    Zero[4] = 0.0 + (0.0 * I);
    Zero[5] = 0.0 + (0.0 * I);
    Zero[6] = 0.0 + (0.0 * I);
    Zero[7] = 0.0 + (0.0 * I);
    Zero[8] = 0.0 + (0.0 * I);
    Zero[9] = 0.0 + (0.0 * I);
    npole = 25;
    Pole[1] = (-0.27287e3) + ((-0.38935e3) * I);
    Pole[2] = (-0.27287e3) + (0.38935e3 * I);
    Pole[3] = (-0.70460e2) + ((-0.41884e2) * I);
    Pole[4] = (-0.70460e2) + (0.41884e2 * I);
    Pole[5] = (-0.19294e3) + (0.0 * I);
    Pole[6] = (-0.33303e3) + (0.0 * I);
    Pole[7] = (-0.28643e-1) + (0.0 * I);
    Pole[8] = (-0.75117e-1) + ((-0.56925e-1) * I);
    Pole[9] = (-0.75117e-1) + (0.56925e-1 * I);
    Pole[10] = (-0.62814e2) + (0.0 * I);
    Pole[11] = (-0.62814e3) + (0.0 * I);
    Pole[12] = (-0.36389e2) + ((-0.18545e2) * I);
    Pole[13] = (-0.36389e2) + (0.18545e2 * I);
    Pole[14] = (-0.28273e1) + (0.0 * I);
    Pole[15] = (-0.18541e2) + ((-0.36391e2) * I);
    Pole[16] = (-0.18541e2) + (0.36391e2 * I);
    Pole[17] = (-0.28273e1) + (0.0 * I);
    Pole[18] = (-0.40342e2) + ((-0.63705e1) * I);
    Pole[19] = (-0.40342e2) + (0.63705e1 * I);
    Pole[20] = (-0.28273e1) + (0.0 * I);
    Pole[21] = (-0.63878e1) + ((-0.40339e2) * I);
    Pole[22] = (-0.63878e1) + (0.40339e2 * I);
    Pole[23] = (-0.28273e1) + (0.0 * I);
    Pole[24] = (-0.28882e2) + ((-0.28877e2) * I);
    Pole[25] = (-0.28882e2) + (0.28877e2 * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

