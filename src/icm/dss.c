
#include "icm.h"
#include "co.h"
#include "complex_sac.h"

void
dss(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_, omo;
    complex double crad;
    complex double pole[8];
    complex double zero[3];
    static double twopi = 6.283185307179586;
    /*   .....LLL DSS - For LLL digital network.....
     *      ( poles and zeros due to P. Rodgers )
     * */

    /*   .....Set poles and zeros.....
     * */
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    omo = twopi / 30.0;
    const_ = 6.152890842e10;
    nzero = 3;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 8;
    crad = csqrt((1.0 - powi(0.707, 2)) + (0.0 * I));
    Pole[1] = (omo + (0. * I)) * (((-0.707) + (0. * I)) + ((0.0 + (1.0 * I)) * crad));
    Pole[2] = (omo + (0. * I)) * (((-0.707) + (0. * I)) - ((0.0 + (1.0 * I)) * crad));
    Pole[3] = (-33.80165) + (60.35942 * I);
    Pole[4] = (-33.80165) + ((-60.35942) * I);
    Pole[5] = (-50.43199) + (35.28386 * I);
    Pole[6] = (-50.43199) + ((-35.28386) * I);
    Pole[7] = (-57.07708) + (11.65531 * I);
    Pole[8] = (-57.07708) + ((-11.65531) * I);
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

