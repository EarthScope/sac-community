
#include "icm.h"
#include "complex_sac.h"

void
clh(int nfreq, double delfrq, double xre[], double xim[])
{
    int npole, nzero;
    float const_;
    /*   .....RSTN CPO LP horizontal seismometer transfer function.....
     * */
    complex double pole[25];
    complex double zero[8];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = (.23249e19 * 7150.0) * 1.0e-8;
    nzero = 8;
    Zero[1] = (-0.10020e3) + (0.0 * I);
    Zero[2] = (-0.60270e1) + (0.0 * I);
    Zero[3] = (-0.18080e4) + (0.0 * I);
    Zero[4] = 0.0 + (0.0 * I);
    Zero[5] = 0.0 + (0.0 * I);
    Zero[6] = 0.0 + (0.0 * I);
    Zero[7] = 0.0 + (0.0 * I);
    Zero[8] = 0.0 + (0.0 * I);
    npole = 25;
    Pole[1] = (-0.20140e5) + (0.0 * I);
    Pole[2] = (-0.18066e4) + (0.0 * I);
    Pole[3] = (-0.40256e3) + (0.0 * I);
    Pole[4] = (-0.20216e3) + (0.0 * I);
    Pole[5] = (-0.16698e2) + ((-0.45746e2) * I);
    Pole[6] = (-0.16698e2) + (0.45746e2 * I);
    Pole[7] = (-0.66753e-1) + ((-0.58761e-1) * I);
    Pole[8] = (-0.66753e-1) + (0.58761e-1 * I);
    Pole[9] = (-0.91893e2) + (0.0 * I);
    Pole[10] = (-0.52330e1) + (0.0 * I);
    Pole[11] = (-0.62814e2) + (0.0 * I);
    Pole[12] = (-0.62814e3) + (0.0 * I);
    Pole[13] = (-0.19582) + ((-0.21692) * I);
    Pole[14] = (-0.19582) + (0.21692 * I);
    Pole[15] = (-0.19582) + ((-0.21692) * I);
    Pole[16] = (-0.19582) + (0.21692 * I);
    Pole[17] = (-0.19725e1) + ((-0.52866) * I);
    Pole[18] = (-0.19725e1) + (0.52866 * I);
    Pole[19] = (-0.14441e1) + ((-0.14438e1) * I);
    Pole[20] = (-0.14441e1) + (0.14438e1 * I);
    Pole[21] = (-0.52856) + ((-0.19725e1) * I);
    Pole[22] = (-0.52856) + (0.19725e1 * I);
    Pole[23] = (-0.31417e-1) + (0.0 * I);
    Pole[24] = (-0.31417e-1) + (0.0 * I);
    Pole[25] = (-0.62832e1) + (0.0 * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

