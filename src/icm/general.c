
#include "icm.h"
#include "co.h"
#include "complex_sac.h"

void
general(int nfreq, double delfrq, double xre[], double xim[], int nzer, double t0, double h, double const_)
{
    int i, npole, nzero;
    float omo;
    complex double crad;
    complex double pole[2];
    complex double zero[3];
    static double twopi = 6.283185307179586;
    /*   .....GENERAL - for a general seismometer.....
     * */

    /*   .....Set poles and zeros.....
     * */
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    nzero = 3;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    nzero = nzer;
    npole = 2;
    omo = twopi / t0;
    crad = csqrt((1.0 - powi(h, 2)) + (0.0 * I));
    Pole[1] = (omo + (0. * I)) * (((-h) + (0. * I)) + ((0.0 + (1.0 * I)) * crad));
    Pole[2] = (omo + (0. * I)) * (((-h) + (0. * I)) - ((0.0 + (1.0 * I)) * crad));
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

