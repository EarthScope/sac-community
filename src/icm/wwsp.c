
#include "icm.h"


void /*FUNCTION*/
wwsp(nfreq, delfrq, xre, xim)
     int nfreq;
     double delfrq, xre[], xim[];
{
    int npole, nzero;
    double const_;
    complexd pole[5], zero[3];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*  WWSSN short period seismometer.
     *  Ref:  Luh, P. C. (1977).  A scheme for expressing instrumental
     *  responses parametrically, BSSA, 67, 957-969.
     * */

    /*   .....Set poles and zeros.....
     * */
    const_ = 397.54767;
    nzero = 3;
    Zero[1] = dbltocmplx(0.0, 0.0);
    Zero[2] = dbltocmplx(0.0, 0.0);
    Zero[3] = dbltocmplx(0.0, 0.0);

    npole = 5;
    Pole[1] = dbltocmplx(-5.0136607, 6.4615109);
    Pole[2] = dbltocmplx(-5.0136607, -6.4615109);
    Pole[3] = dbltocmplx(-8.2981509, 0.0);
    Pole[4] = dbltocmplx(-8.6940765, -7.1968661);
    Pole[5] = dbltocmplx(-8.6940765, 7.1968661);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
