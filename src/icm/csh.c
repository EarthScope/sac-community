
#include "icm.h"

void /*FUNCTION*/
csh(int nfreq, double delfrq, double xre[], double xim[])
{
    int npole, nzero;
    double const_;
    complexd pole[26], zero[10];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....RSTN CPO SP horizontal seismometer transfer function.....
     * */
    const_ = .38966e33 * 42000.0 * 1.0e-8;

    nzero = 10;
    Zero[1] = dbltocmplx(-0.10020e3, 0.0);
    Zero[2] = dbltocmplx(-0.60270e1, 0.0);
    Zero[3] = dbltocmplx(-0.18080e4, 0.0);
    Zero[4] = dbltocmplx(0.0, 0.0);
    Zero[5] = dbltocmplx(0.0, 0.0);
    Zero[6] = dbltocmplx(0.0, 0.0);
    Zero[7] = dbltocmplx(0.0, 0.0);
    Zero[8] = dbltocmplx(0.0, 0.0);
    Zero[9] = dbltocmplx(0.0, 0.0);
    Zero[10] = dbltocmplx(0.0, 0.0);

    npole = 26;
    Pole[1] = dbltocmplx(-0.20140e5, 0.0);
    Pole[2] = dbltocmplx(-0.18066e4, 0.0);
    Pole[3] = dbltocmplx(-0.40256e3, 0.0);
    Pole[4] = dbltocmplx(-0.20216e3, 0.0);
    Pole[5] = dbltocmplx(-0.16698e2, -0.45746e2);
    Pole[6] = dbltocmplx(-0.16698e2, 0.45746e2);
    Pole[7] = dbltocmplx(-0.66753e-1, -0.58761e-1);
    Pole[8] = dbltocmplx(-0.66753e-1, 0.58761e-1);
    Pole[9] = dbltocmplx(-0.91893e2, 0.0);
    Pole[10] = dbltocmplx(-0.52330e1, 0.0);
    Pole[11] = dbltocmplx(-0.62814e2, 0.0);
    Pole[12] = dbltocmplx(-0.62814e3, 0.0);
    Pole[13] = dbltocmplx(-0.36389e2, -0.18545e2);
    Pole[14] = dbltocmplx(-0.36389e2, 0.18545e2);
    Pole[15] = dbltocmplx(-0.28273e1, 0.0);
    Pole[16] = dbltocmplx(-0.18541e2, -0.36391e2);
    Pole[17] = dbltocmplx(-0.18541e2, 0.36391e2);
    Pole[18] = dbltocmplx(-0.28273e1, 0.0);
    Pole[19] = dbltocmplx(-0.40342e2, -0.63705e1);
    Pole[20] = dbltocmplx(-0.40342e2, 0.63705e1);
    Pole[21] = dbltocmplx(-0.28273e1, 0.0);
    Pole[22] = dbltocmplx(-0.63878e1, -0.40339e2);
    Pole[23] = dbltocmplx(-0.63878e1, 0.40339e2);
    Pole[24] = dbltocmplx(-0.28273e1, 0.0);
    Pole[25] = dbltocmplx(-0.28882e2, -0.28877e2);
    Pole[26] = dbltocmplx(-0.28882e2, 0.28877e2);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
