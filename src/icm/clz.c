
#include "icm.h"

void /*FUNCTION*/
clz(int nfreq, double delfrq, double xre[], double xim[])
{
    int npole, nzero;
    double const_;
    complexd pole[24], zero[7];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....RSTN CPO LP vertical seismometer transfer function.....
     * */
    const_ = .20121e18 * 9000.0 * 1.0e-8;

    nzero = 7;
    Zero[1] = dbltocmplx(-0.37700e3, 0.0);
    Zero[2] = dbltocmplx(0.0, 0.0);
    Zero[3] = dbltocmplx(0.0, 0.0);
    Zero[4] = dbltocmplx(0.0, 0.0);
    Zero[5] = dbltocmplx(0.0, 0.0);
    Zero[6] = dbltocmplx(0.0, 0.0);
    Zero[7] = dbltocmplx(0.0, 0.0);

    npole = 24;
    Pole[1] = dbltocmplx(-0.27287e3, -0.38935e3);
    Pole[2] = dbltocmplx(-0.27287e3, 0.38935e3);
    Pole[3] = dbltocmplx(-0.70460e2, -0.41884e2);
    Pole[4] = dbltocmplx(-0.70460e2, 0.41884e2);
    Pole[5] = dbltocmplx(-0.19294e3, 0.0);
    Pole[6] = dbltocmplx(-0.33303e3, 0.0);
    Pole[7] = dbltocmplx(-0.28643e-1, 0.0);
    Pole[8] = dbltocmplx(-0.75117e-1, -0.56925e-1);
    Pole[9] = dbltocmplx(-0.75117e-1, 0.56925e-1);
    Pole[10] = dbltocmplx(-0.62814e2, 0.0);
    Pole[11] = dbltocmplx(-0.62814e3, 0.0);
    Pole[12] = dbltocmplx(-0.19582, -0.21692);
    Pole[13] = dbltocmplx(-0.19582, 0.21692);
    Pole[14] = dbltocmplx(-0.19582, -0.21692);
    Pole[15] = dbltocmplx(-0.19582, 0.21692);
    Pole[16] = dbltocmplx(-0.19725e1, -0.52866);
    Pole[17] = dbltocmplx(-0.19725e1, 0.52866);
    Pole[18] = dbltocmplx(-0.14441e1, -0.14438e1);
    Pole[19] = dbltocmplx(-0.14441e1, 0.14438e1);
    Pole[20] = dbltocmplx(-0.52856, -0.19725e1);
    Pole[21] = dbltocmplx(-0.52856, 0.19725e1);
    Pole[22] = dbltocmplx(-0.31417e-1, 0.0);
    Pole[23] = dbltocmplx(-0.31417e-1, 0.0);
    Pole[24] = dbltocmplx(-0.62832e1, 0.0);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
