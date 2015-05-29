
#include "icm.h"

#include "complex_sac.h"

void /*FUNCTION*/
acc(nfreq, delfrq, xre, xim)
     int nfreq;
     double delfrq, xre[], xim[];
{
    int i, npole, nzero;
    float const_;
    double complex pole[2], zero[2];

    double complex *const Zero = &zero[0] - 1;

    /*   .....Acceleration Spectral Operator.....
     *
     *
     *   .....Set poles and zeros.....
     * */
    const_ = 1.0;
    npole = 0;
    nzero = 2;

    for (i = 1; i <= nzero; i++) {
        Zero[i] = 0.0 + 0.0 * I;
    }

    /*   .....Compute transfer function.....
     * */


    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}
