
#include "icm.h"
#include "complex_sac.h"

void
rsm(int nfreq, double delfrq, double xre[], double xim[])
{
    int npole, nzero;
    float const_;
    /*   .....RSTN MP seismometer transfer function.....
     * */
    const_ = 0.79601e27 * 34700.0 * 1.0e-8;

    complex double pole[24];
    complex double zero[7];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    nzero = 7;
    Zero[1] = (-0.10067e4) + (0.0 * I);
    Zero[2] = 0.0 + (0.0 * I);
    Zero[3] = 0.0 + (0.0 * I);
    Zero[4] = 0.0 + (0.0 * I);
    Zero[5] = 0.0 + (0.0 * I);
    Zero[6] = 0.0 + (0.0 * I);
    Zero[7] = 0.0 + (0.0 * I);
    npole = 24;
    Pole[1] = (-0.12272e3) + ((-0.16843e3) * I);
    Pole[2] = (-0.12272e3) + (0.16843e3 * I);
    Pole[3] = (-0.13505e3) + (0.0 * I);
    Pole[4] = (-0.11287e4) + (0.0 * I);
    Pole[5] = (-0.12895) + (0.0 * I);
    Pole[6] = (-0.17218e2) + (0.0 * I);
    Pole[7] = (-0.74713e-1) + ((-0.57094e-1) * I);
    Pole[8] = (-0.74713e-1) + (0.57094e-1 * I);
    Pole[9] = (-0.12566e4) + (0.0 * I);
    Pole[10] = (-0.62814e3) + (0.0 * I);
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

