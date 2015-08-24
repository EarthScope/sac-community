
#include <math.h>

#include "icm.h"

void /*FUNCTION*/
reftek(nfreq, delfrq, xre, xim, freepd, damp, crfrq, hpfrq)
     int nfreq;
     double delfrq, xre[], xim[], freepd, damp, crfrq, hpfrq;
{
    int npole, nzero;
    double dc, discrm, fp, s1i, s1r, s2i, s2r, sfil1i, sfil1r, sfil2i, sfil2r,
        sfil3i, sfil3r, temp;
    double cf, cfTo6th;
    complexd pole[9], zero[4];
    static double twopi = M_PI * 2;

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

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
    Zero[1] = dbltocmplx(0.0, 0.0);
    Zero[2] = dbltocmplx(0.0, 0.0);
    Zero[3] = dbltocmplx(0.0, 0.0);

    npole = 8;
    /*     Butterworth filter, 6 poles */
    Pole[1] = dbltocmplx(sfil1r, sfil1i);
    Pole[2] = dbltocmplx(sfil2r, sfil2i);
    Pole[3] = dbltocmplx(sfil3r, sfil3i);
    Pole[4] = dbltocmplx(sfil1r, -sfil1i);
    Pole[5] = dbltocmplx(sfil2r, -sfil2i);
    Pole[6] = dbltocmplx(sfil3r, -sfil3i);
    /*     Seismometer poles */
    Pole[7] = dbltocmplx(s1r, s1i);
    Pole[8] = dbltocmplx(s2r, s2i);
    /*     The high pass filter if present..... */
    if (hpfrq > 0.0) {
        nzero = 4;
        npole = 9;
        Zero[4] = dbltocmplx(0.0, 0.0);
        temp = -twopi * hpfrq;
        Pole[9] = dbltocmplx(temp, 0.0);
    }

    getrand(nfreq, delfrq, cfTo6th, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
