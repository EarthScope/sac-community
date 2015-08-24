
#include "icm.h"


void /*FUNCTION*/
wwlpbn(nfreq, delfrq, xre, xim)
     int nfreq;
     double delfrq, xre[], xim[];
{
    int npole, nzero;
    double const_;
    complexd pole[4], zero[3];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*  .....WWSSN LP - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 0.5985275       <*    AMP of 1.0 at FREQ of 0.05 Hz */
    const_ = 0.5985275;

    nzero = 3;
    Zero[1] = dbltocmplx(0.0, 0.0);
    Zero[2] = dbltocmplx(0.0, 0.0);
    Zero[3] = dbltocmplx(0.0, 0.0);

    npole = 4;
    Pole[1] = dbltocmplx(-0.257, 0.3376);
    Pole[2] = dbltocmplx(-0.257, -0.3376);
    Pole[3] = dbltocmplx(-0.06283, 0.0);
    Pole[4] = dbltocmplx(-0.06283, 0.0);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
