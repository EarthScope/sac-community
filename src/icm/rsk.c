
#include <string.h>

#include "icm.h"


void /*FUNCTION*/
rsk(nfreq, delfrq, xre, xim, subtyp)
     int nfreq;
     double delfrq, xre[], xim[];
     char *subtyp;
{
    int npole, nzero;
    double const_;
    complexd pole[16], zero[9];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....RSTN KS-36000 SP seismometer transfer function.....
     * */
    const_ = 0.249e18 * 32000.0 * 1.0e-8;
    if (memcmp(subtyp, "ONKS.N  ", 8) == 0)
        const_ = 0.5 * const_;

    nzero = 9;
    Zero[1] = dbltocmplx(-0.10067e4, 0.0);
    Zero[2] = dbltocmplx(-0.40078e3, 0.0);
    Zero[3] = dbltocmplx(0.0, 0.0);
    Zero[4] = dbltocmplx(0.0, 0.0);
    Zero[5] = dbltocmplx(0.0, 0.0);
    Zero[6] = dbltocmplx(0.0, 0.0);
    Zero[7] = dbltocmplx(0.0, 0.0);
    Zero[8] = dbltocmplx(0.0, 0.0);
    Zero[9] = dbltocmplx(0.0, 0.0);

    npole = 16;
    Pole[1] = dbltocmplx(-0.12272e3, -0.16843e3);
    Pole[2] = dbltocmplx(-0.12272e3, 0.16843e3);
    Pole[3] = dbltocmplx(-0.13505e3, 0.0);
    Pole[4] = dbltocmplx(-0.11287e4, 0.0);
    Pole[5] = dbltocmplx(-0.99716, -0.76548);
    Pole[6] = dbltocmplx(-0.99716, 0.76548);
    Pole[7] = dbltocmplx(-0.12566e4, 0.0);
    Pole[8] = dbltocmplx(-0.62814e3, 0.0);
    Pole[9] = dbltocmplx(-0.28273e1, 0.0);
    Pole[10] = dbltocmplx(-0.28273e1, 0.0);
    Pole[11] = dbltocmplx(-0.28273e1, 0.0);
    Pole[12] = dbltocmplx(-0.28273e1, 0.0);
    Pole[13] = dbltocmplx(-0.86194e2, -0.25840e2);
    Pole[14] = dbltocmplx(-0.86194e2, 0.25840e2);
    Pole[15] = dbltocmplx(-0.62640e2, -0.79078e2);
    Pole[16] = dbltocmplx(-0.62640e2, 0.79078e2);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
