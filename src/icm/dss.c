
#include "icm.h"
#include "co.h"


void /*FUNCTION*/
dss(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    double const_, omo;
    complexd crad, pole[8], zero[3];
    static double twopi = 6.283185307179586;

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....LLL DSS - For LLL digital network.....
     *      ( poles and zeros due to P. Rodgers )
     * */

    /*   .....Set poles and zeros.....
     * */
    omo = twopi / 30.0;
    const_ = 6.152890842e10;
    nzero = 3;
    for (i = 1; i <= nzero; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }

    npole = 8;
    crad = dcmplxsqrt(dbltocmplx(1.0 - powi(0.707, 2), 0.0));
    Pole[1] =
        dcmplxmul(dbltocmplx(omo, 0.),
                 (dcmplxadd
                  (dbltocmplx(-0.707, 0.),
                   dcmplxmul(dbltocmplx(0.0, 1.0), crad))));
    Pole[2] =
        dcmplxmul(dbltocmplx(omo, 0.),
                 (dcmplxsub
                  (dbltocmplx(-0.707, 0.),
                   dcmplxmul(dbltocmplx(0.0, 1.0), crad))));
    Pole[3] = dbltocmplx(-33.80165, 60.35942);
    Pole[4] = dbltocmplx(-33.80165, -60.35942);
    Pole[5] = dbltocmplx(-50.43199, 35.28386);
    Pole[6] = dbltocmplx(-50.43199, -35.28386);
    Pole[7] = dbltocmplx(-57.07708, 11.65531);
    Pole[8] = dbltocmplx(-57.07708, -11.65531);

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
