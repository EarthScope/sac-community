
#include "icm.h"
#include "co.h"


void /*FUNCTION*/
general(nfreq, delfrq, xre, xim, nzer, t0, h, const_)
     int nfreq;
     double delfrq, xre[], xim[];
     int nzer;
     double t0, h, const_;
{
    int i, npole, nzero;
    double omo;
    complexd crad, pole[2], zero[3];
    static double twopi = M_PI * 2;

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....GENERAL - for a general seismometer.....
     * */

    /*   .....Set poles and zeros.....
     * */
    nzero = 3;
    for (i = 1; i <= nzero; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }
    nzero = nzer;

    npole = 2;
    omo = twopi / t0;
    crad = dcmplxsqrt(dbltocmplx(1.0 - powi(h, 2), 0.0));
    Pole[1] =
        dcmplxmul(dbltocmplx(omo, 0.),
                 (dcmplxadd
                  (dbltocmplx(-h, 0.), dcmplxmul(dbltocmplx(0.0, 1.0), crad))));
    Pole[2] =
        dcmplxmul(dbltocmplx(omo, 0.),
                 (dcmplxsub
                  (dbltocmplx(-h, 0.), dcmplxmul(dbltocmplx(0.0, 1.0), crad))));

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
