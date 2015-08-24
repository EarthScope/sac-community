
#include <string.h>

#include "icm.h"


void /*FUNCTION*/
rs7(nfreq, delfrq, xre, xim, subtyp)
     int nfreq;
     double delfrq, xre[], xim[];
     char *subtyp;
{
    int npole, nzero;
    double const_;
    complexd pole[20], zero[13];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....RSTN S-750 SP seismometer transfer function.....
     * */
    const_ = -0.10729e20 * 32000.0 * 1.0e-8;
    if (memcmp(subtyp, "ON7S.E  ", 8) == 0)
        const_ = -const_;

    nzero = 13;
    Zero[1] = dbltocmplx(-0.37370e3, 0.0);
    Zero[2] = dbltocmplx(0.0, 0.0);
    Zero[3] = dbltocmplx(0.0, 0.0);
    Zero[4] = dbltocmplx(0.0, 0.0);
    Zero[5] = dbltocmplx(0.0, 0.0);
    Zero[6] = dbltocmplx(0.0, 0.0);
    Zero[7] = dbltocmplx(-0.11480e4, 0.0);
    Zero[8] = dbltocmplx(-0.65050e4, 0.0);
    Zero[9] = dbltocmplx(-0.78344e5, 0.0);
    Zero[10] = dbltocmplx(-0.21120e6, 0.0);
    Zero[11] = dbltocmplx(0.0, 0.0);
    Zero[12] = dbltocmplx(0.0, 0.0);
    Zero[13] = dbltocmplx(0.0, 0.0);

    npole = 20;
    Pole[1] = dbltocmplx(-0.78828e5, 0.0);
    Pole[2] = dbltocmplx(-0.49991e5, 0.0);
    Pole[3] = dbltocmplx(-0.99000e4, 0.0);
    Pole[4] = dbltocmplx(-0.67240e4, 0.0);
    Pole[5] = dbltocmplx(-0.26310e3, -0.40670e3);
    Pole[6] = dbltocmplx(-0.26310e3, 0.40670e3);
    Pole[7] = dbltocmplx(-0.53020e3, 0.0);
    Pole[8] = dbltocmplx(-0.62500e-1, 0.0);
    Pole[9] = dbltocmplx(-0.99720, -0.76530);
    Pole[10] = dbltocmplx(-0.99720, 0.76530);
    Pole[11] = dbltocmplx(-0.12566e4, 0.0);
    Pole[12] = dbltocmplx(-0.62814e3, 0.0);
    Pole[13] = dbltocmplx(-0.28270e1, 0.0);
    Pole[14] = dbltocmplx(-0.28270e1, 0.0);
    Pole[15] = dbltocmplx(-0.28270e1, 0.0);
    Pole[16] = dbltocmplx(-0.28270e1, 0.0);
    Pole[17] = dbltocmplx(-0.86190e2, -0.25840);
    Pole[18] = dbltocmplx(-0.86190e2, 0.25840);
    Pole[19] = dbltocmplx(-0.62640e2, -0.79080e2);
    Pole[20] = dbltocmplx(-0.62640e2, 0.79080e2);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
