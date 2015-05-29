
#include <math.h>

#include "icm.h"
#include "complex_sac.h"

void
reftek(int nfreq, double delfrq, double xre[], double xim[], double freepd, double damp, double crfrq, double hpfrq)
{
    int npole, nzero;
    float dc, discrm, fp, s1i, s1r, s2i, s2r, sfil1i, sfil1r, sfil2i, sfil2r,
        sfil3i, sfil3r, temp;
    double cf, cfTo6th;
    complex double pole[9];
    complex double zero[4];
    static float twopi = 6.283185307179586;
    /*   .....REFTEK - for seismometer-REFTEK box response.....
     *             (poles and zeros due to G. Randall, modified version
     *              port routine original by H. Patton)
     *
     * MODIFICATIONS:
     *    901012:  Deleted the 1.0e-07 scale factor from the displacement
     *             response constant. (wct)
     *    900409:  Deleted the gain argument and coding.
     * */

    /*     fp = twopi / freepd       <*   (Radians) */
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    fp = twopi / freepd;
    dc = damp;
    /*     cf = twopi * crfrq        <*   Corner Frequency ( Radians ) */
    cf = twopi * crfrq;

    /*   .....Six pole Butterworth filter.....
     *        Upper left quadrant poles, lower left by symmetry
     * */
    sfil1r = -cf * sin(twopi / 24);
    sfil1i = cf * cos(twopi / 24);
    sfil2r = -cf * sin(twopi * 3 / 24);
    sfil2i = cf * cos(twopi * 3 / 24);
    sfil3r = -sfil1i;
    sfil3i = -sfil1r;

    /*   Seismometer
     *    Allow for variable seismometer free period and damping constant.
     *    Numerator: S*S ( for velocitygram ).
     *    Poles are roots of denominator:
     *                 2                 2
     *              (S) + 2DC(FP)S + (FP) = 0.0
     *         Where:
     *                 S = i * omega
     *                DC = damping constant.
     *                FP = 2 Pi * ( free period in sec. )    <*   (radians)
     * */
    discrm = dc * dc - 1.0;
    if (discrm > 0.0) {
        /* .....Overdamped..... */
        discrm = sqrt(discrm);
        s1r = fp * (-dc + discrm);
        s1i = 0.0;
        s2r = fp * (-dc - discrm);
        s2i = 0.0;

    } else {
        discrm = fabs(discrm);
        discrm = sqrt(discrm);
        s1r = -dc * fp;
        s1i = fp * discrm;
        s2r = s1r;
        s2i = -s1i;
    }

    /*   .....computing displacement response.....
     *       if the hpfrq > 0., then add a single zero at D.C. and
     *       a single pole at the high pass filter freq
     * */
    cfTo6th = cf * cf * cf * cf * cf * cf;
    nzero = 3;
    Zero[1] = 0.0 + (0.0 * I);
    Zero[2] = 0.0 + (0.0 * I);
    Zero[3] = 0.0 + (0.0 * I);
    npole = 8;
    /*     Butterworth filter, 6 poles */
    Pole[1] = sfil1r + (sfil1i * I);
    Pole[2] = sfil2r + (sfil2i * I);
    Pole[3] = sfil3r + (sfil3i * I);
    Pole[4] = sfil1r + ((-sfil1i) * I);
    Pole[5] = sfil2r + ((-sfil2i) * I);
    Pole[6] = sfil3r + ((-sfil3i) * I);
    /*     Seismometer poles */
    Pole[7] = s1r + (s1i * I);
    Pole[8] = s2r + (s2i * I);
    /*     The high pass filter if present..... */
    if (hpfrq > 0.0) {
        nzero = 4;
        npole = 9;
        Zero[4] = 0.0 + (0.0 * I);
        temp = (-twopi) * hpfrq;
        Pole[9] = temp + (0.0 * I);
    }

    getranx(nfreq, delfrq, cfTo6th, nzero, zero, npole, pole, xre, xim);
    return;
}

