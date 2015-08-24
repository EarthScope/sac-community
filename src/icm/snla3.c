
#include <math.h>

#include "icm.h"
#include "co.h"

void
snla3(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, j, npole, nzero;
    double anorm, asqrd, astest, cf3db, const_, delomg, omega, omega0, prd1,
        prd2, prd3, prd4;
    double fac, ti, ti0, tid, tin, tr, tr0, trd, trn, xxim, xxre;
    complexd pole[10];
    complexd zero[2];
    static double twopi = 6.283185307179586;

    complexd * const Pole = (&pole[0]) - 1;
    double * const Xim = (&xim[0]) - 1;
    double * const Xre = (&xre[0]) - 1;
    complexd * const Zero = (&zero[0]) - 1;
    /*  Response due to H. Patton
     *
     *   new system response (1985) of Sandia Network ---
     *   sl-250 Geotech Seismometers are set at 20 sec.
     *   natural period and critically damped.  Signal is
     *   low-pass filtered with an eight-pole Butterworth
     *   filter with a 3 db point at 10 Hz.  Sampling rate
     *   is 50 samples/sec.
     * */

    delomg = twopi * delfrq;
    const_ = 1.0;

    /*   .....Eight-pole Butterworth Filter.....
     * */
    cf3db = twopi * 10.;
    prd1 = 0.9808 * cf3db;
    prd2 = 0.1951 * cf3db;
    prd3 = 0.8315 * cf3db;
    prd4 = 0.5556 * cf3db;

    /*   .....Critically damped seismometer with t = 20 sec.....
     * */
    Pole[1] = dbltocmplx((-prd1),  (prd2 ));
    Pole[2] = dbltocmplx((-prd1),  ((-prd2) ));
    Pole[3] = dbltocmplx((-prd3),  (prd4 ));
    Pole[4] = dbltocmplx((-prd3),  ((-prd4) ));
    Pole[5] = dbltocmplx((-prd2),  (prd1 ));
    Pole[6] = dbltocmplx((-prd2),  ((-prd1) ));
    Pole[7] = dbltocmplx((-prd4),  (prd3 ));
    Pole[8] = dbltocmplx((-prd4), ((-prd3) ));
    omega0 = twopi / 20.;

    Pole[9] = dbltocmplx(-omega0, 0.0);
    Pole[10] = dbltocmplx(-omega0, 0.0);
    npole = 10;

    /*   .....Computing velocity response.....
     * */
    nzero = 2;

    Zero[1] = dbltocmplx(0,0);
    Zero[2] = dbltocmplx(0,0);

    asqrd = 0.0;
    for (j = 1; j <= nfreq; j++) {
        omega = delomg * (double) (j - 1);
        trn = 1.0e0;
        tin = 0.0e0;
        for (i = 1; i <= nzero; i++) {
            tr = -(Zero[i].re);
            ti = omega - (Zero[i].im);
            tr0 = (trn * tr) - (tin * ti);
            ti0 = (trn * ti) + (tin * tr);
            trn = tr0;
            tin = ti0;
        }

        trd = 1.0e0;
        tid = 0.0e0;
        for (i = 1; i <= npole; i++) {
            tr = -(Pole[i].re);
            ti = omega - (Pole[i].im);
            tr0 = (trd * tr) - (tid * ti);
            ti0 = (trd * ti) + (tid * tr);
            trd = tr0;
            tid = ti0;
        }

        fac = ((double) const_) / (pow(trd, 2) + pow(tid, 2));
        Xre[j] = fac * ((trn * trd) + (tin * tid));
        Xim[j] = fac * ((trd * tin) - (trn * tid));
        astest = pow(Xre[j], 2) + pow(Xim[j], 2);
        if (astest > asqrd)
            asqrd = astest;
    }

    /*   .....Normalization such that velocity tranfer function
     *      has maximum equal to unity i. e. displacement transfer
     *      function has gain equal to 2*pi*fmax where fmax is
     *      maximum point on velocity transfer function.....
     *
     *   .....Also transform velocity response to displacement
     *      response -- multiply by i * omega.....
     * */
    anorm = 1.0 / sqrt(asqrd);

    for (j = 1; j <= nfreq; j++) {
        omega = delomg * ((double) (j - 1));
        xxre = (-Xim[j]) * ((double) (omega * anorm));
        xxim = Xre[j] * ((double) (omega * anorm));
        Xre[j] = xxre;
        Xim[j] = xxim;
    }

    return;
}

