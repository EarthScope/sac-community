
#include "icm.h"
#include "complex_sac.h"

void
vel(int nfreq, double delfrq, double xre[], double xim[])
{
    int npole, nzero;
    float const_;
    complex double pole[1];
    complex double zero[1];
    complex double * const Zero = (&zero[0]) - 1;
    /*   .....VEL - velocity spectral operator.....
     * */

    /*   .....Set poles and zeros.....
     * */
    const_ = 1.0;
    nzero = 1;

    Zero[1] = 0.0 + (0.0 * I);

    npole = 0;

    /*   .....Compute transfer function.....
     * */
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

