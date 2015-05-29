
#include "icm.h"
#include "complex_sac.h"

void
cmz(int nfreq, double delfrq, double xre[], double xim[])
{
    int npole, nzero;
    float const_;
    /*   .....RSTN CPO MP vertical seismometer transfer function.....
     * */
    complex double pole[24];
    complex double zero[7];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = (.13703e27 * 41500.0) * 1.0e-8;
    nzero = 7;
    Zero[1] = (-0.37700e3) + (0.0 * I);
    Zero[2] = 0.0 + (0.0 * I);
    Zero[3] = 0.0 + (0.0 * I);
    Zero[4] = 0.0 + (0.0 * I);
    Zero[5] = 0.0 + (0.0 * I);
    Zero[6] = 0.0 + (0.0 * I);
    Zero[7] = 0.0 + (0.0 * I);
    npole = 24;
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
    Pole[12] = (-0.80650e1) + ((-0.12880e1) * I);
    Pole[13] = (-0.80650e1) + (0.12880e1 * I);
    Pole[14] = (-0.72780e1) + ((-0.37070e1) * I);
    Pole[15] = (-0.72780e1) + (0.37070e1 * I);
    Pole[16] = (-0.57740e1) + ((-0.57770e1) * I);
    Pole[17] = (-0.57740e1) + (0.57770e1 * I);
    Pole[18] = (-0.37090e1) + ((-0.72770e1) * I);
    Pole[19] = (-0.37090e1) + (0.72770e1 * I);
    Pole[20] = (-0.12770e1) + ((-0.80670e1) * I);
    Pole[21] = (-0.12770e1) + (0.80670e1 * I);
    Pole[22] = (-0.31450e-1) + (0.0 * I);
    Pole[23] = (-0.31450e-1) + (0.0 * I);
    Pole[24] = (-0.62814e2) + (0.0 * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

