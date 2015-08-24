
#include "icm.h"


void /*FUNCTION*/
llsn(nfreq, delfrq, xre, xim)
     int nfreq;
     double delfrq, xre[], xim[];
{
    int i, npole, nzero;
    double const_;
    complexd pole[10], zero[5];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....LLSN - for an L-4 seismometer.....
     *     (poles and zeros due to P. Rodgers)
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = -3.57425 e15   <*  = -A * B */
    const_ = -3.57425e15;
    /*     nzero = 5              <* checked on 2/2/81 */
    nzero = 5;

    for (i = 1; i <= nzero; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }

    npole = 10;
    Pole[1] = dbltocmplx(-5.026548, 3.769911);
    Pole[2] = dbltocmplx(-5.026548, -3.769911);
    Pole[3] = dbltocmplx(-0.5969026, 0.0);
    Pole[4] = dbltocmplx(-0.5969026, 0.0);
    Pole[5] = dbltocmplx(-276.460154, 0.0);
    Pole[6] = dbltocmplx(-276.460154, 0.0);
    Pole[7] = dbltocmplx(-376.99112, 0.0);
    Pole[8] = dbltocmplx(-376.99112, 0.0);
    Pole[9] = dbltocmplx(-571.76986, 583.32194);
    Pole[10] = dbltocmplx(-571.76986, -583.32194);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
