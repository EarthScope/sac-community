
#include "icm.h"


void /*FUNCTION*/
redkir(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    double const_;
    complexd pole[4], zero[3];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....RED KIRN - Blacknest specified poles and zeros.....
     * */

    /*   .....Set poles and zeros.....
     *
     *     const = 83.692405956 e0  <*  AMP of 1.0 at FREQ of 1.0 Hz */
    const_ = 83.692405956;
    nzero = 3;

    for (i = 1; i <= nzero; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }

    npole = 4;
    Pole[1] = dbltocmplx(-0.12759, 0.23031);
    Pole[2] = dbltocmplx(-0.12759, -0.23031);
    Pole[3] = dbltocmplx(-0.29915, 0.0);
    Pole[4] = dbltocmplx(-83.43929, 0.0);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
