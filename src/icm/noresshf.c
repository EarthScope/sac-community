
#include "icm.h"


void /*FUNCTION*/
noresshf(int nfreq, double delfrq, double xre[], double xim[])
{
    int i;
    double delomg, omega;
    complexd chhf, chhp, chia, chif, chlp, chpa, chsi, cs, temp1, temp2, temp3;
    static double twopi = 6.283185307179586;

    double *const Xim = &xim[0] - 1;
    double *const Xre = &xre[0] - 1;

    /*   .....NORESS high frequency element (Durham, 6 March 1986).....
     *          output in volts/nm
     *          A. Smith, 6 May 1986
     * */

    delomg = twopi * delfrq;

    for (i = 1; i <= nfreq; i++) {
        omega = (double) (i - 1) * delomg;
        cs = dbltocmplx(0.0, omega);
        chsi =
            dcmplxdiv(dcmplxmul(dbltocmplx(55.73, 0.), dcmplxpow(cs, (double) 2)),
                     (dcmplxadd
                      (dcmplxadd
                       (dbltocmplx(1.0, 0.),
                        dcmplxmul(dbltocmplx(0.2387, 0.), cs)),
                       dcmplxmul(dbltocmplx(0.02533, 0.),
                                dcmplxpow(cs, (double) 2)))));
        chif =
            dcmplxdiv(dbltocmplx(0.9174, 0.),
                     (dcmplxadd
                      (dcmplxadd
                       (dbltocmplx(1.0, 0.),
                        dcmplxmul(dbltocmplx(8.165e-4, 0.), cs)),
                       dcmplxmul(dbltocmplx(7.339e-7, 0.),
                                dcmplxpow(cs, (double) 2)))));
        chpa =
            dcmplxdiv(dcmplxmul(dbltocmplx(5.544, 0.), cs),
                     (dcmplxmul
                      ((dcmplxadd
                        (dbltocmplx(1.0, 0.),
                         dcmplxmul(dbltocmplx(1.559e-3, 0.), cs))),
                       (dcmplxadd
                        (dbltocmplx(1.0, 0.),
                         dcmplxmul(dbltocmplx(0.1598, 0.), cs))))));
        chia =
            dcmplxdiv(dcmplxmul(dbltocmplx(0.3186, 0.), cs),
                     (dcmplxmul
                      ((dcmplxadd
                        (dbltocmplx(1.0, 0.),
                         dcmplxmul(dbltocmplx(1.593, 0.), cs))),
                       (dcmplxadd
                        (dbltocmplx(1.0, 0.),
                         dcmplxmul(dbltocmplx(2.496e-3, 0.), cs))))));

        /* Use of temporary variables below due to bug in MASSCOMP compiler (870528) */

        temp1 = dcmplxmul(cs, cs);
        temp2 = dcmplxmul(dbltocmplx(0.0124, 0.), temp1);
        temp3 =
            (dcmplxmul
             ((dcmplxadd
               (dbltocmplx(1.0, 0.), dcmplxmul(dbltocmplx(0.0500, 0.), cs))),
              (dcmplxadd
               (dbltocmplx(1.0, 0.), dcmplxmul(dbltocmplx(0.124, 0.), cs)))));
        chhp = dcmplxdiv(temp2, temp3);

        chlp =
            dcmplxdiv(dbltocmplx(1.0, 0.),
                     (dcmplxadd
                      (dbltocmplx(1.0, 0.),
                       dcmplxpow((dcmplxmul(dbltocmplx(2.894e-3, 0.), cs)),
                                (double) 36))));

        chhf =
            dcmplxmul(dcmplxmul
                     (dcmplxmul
                      (dcmplxmul
                       (dcmplxmul(dcmplxmul(dcmplxmul(chsi, chif), chpa), chia),
                        chhp), chlp), dbltocmplx(1.0e-9, 0.)), cs);

        Xre[i] = dcmplxtof(chhf);
        Xim[i] = daimag(chhf);
    }
    return;
}                               /* end of function */
