
#include "icm.h"


void /*FUNCTION*/
eyeomg(int nfreq, double delfrq, double xre[], double xim[], int nzer)
{
    int i, npole;
    double const_;
    complexd pole[30], zero[30];

    complexd *const Zero = &zero[0] - 1;

    /*   .....I - Omega.....
     *
     *
     *   .....Set poles and zeros.....
     * */
    const_ = 1.0;
    npole = 0;

    for (i = 1; i <= nzer; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzer, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
