
#include "icm.h"
#include "complex_sac.h"

void
eyeomg(int nfreq, double delfrq, double xre[], double xim[], int nzer)
{
    int i, npole;
    float const_;
    /*   .....I - Omega.....
     *
     *
     *   .....Set poles and zeros.....
     * */
    complex double pole[30];
    complex double zero[1];
    complex double * const Zero = (&zero[0]) - 1;
    const_ = 1.0;
    npole = 0;
    for (i = 1; i <= nzer; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    getranx(nfreq, delfrq, const_, nzer, zero, npole, pole, xre, xim);
    return;
}

