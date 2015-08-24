
#include "icm.h"


void /*FUNCTION*/
hs3(nfreq, delfrq, xre, xim)
     int nfreq;
     double delfrq, xre[], xim[];
{
    int i;
    double delomg, omega;
    complexd cf1, cf2, cha, cha1, cha2, chb, chhp, chs3, cht, cs;
    static double twopi = 6.283185307179586;

    double *const Xim = &xim[0] - 1;
    double *const Xre = &xre[0] - 1;

    /*   .....S-750 Seismometer Response - output is in units of volts/nm....
     * */

    delomg = twopi * delfrq;

    /*   .....Set poles and zeros.....
     * */
    for (i = 1; i <= nfreq; i++) {
        omega = (double) (i - 1) * delomg;
        cs = dbltocmplx(0.0, omega);
        cht =
            dcmplxdiv(dbltocmplx(2.6e3, 0.),
                     (dcmplxadd
                      (dcmplxadd
                       (dbltocmplx(1.11e5, 0.),
                        dcmplxmul(dbltocmplx(33.3, 0.), cs)), dcmplxpow(cs,
                                                                      (double)
                                                                      2))));
        chhp =
            dcmplxdiv(dcmplxmul(dbltocmplx(16.0, 0.), cs),
                     (dcmplxadd
                      (dbltocmplx(1.0, 0.),
                       dcmplxmul(dbltocmplx(1.0e-4, 0.), cs))));
        cha1 =
            dcmplxdiv((dcmplxadd
                      (dbltocmplx(21.0, 0.),
                       dcmplxmul(dbltocmplx(1.0e-4, 0.), cs))),
                     (dcmplxadd
                      (dbltocmplx(1.0, 0.),
                       dcmplxmul(dbltocmplx(1.0e-4, 0.), cs))));
        cha2 =
            dcmplxneg(dcmplxdiv
                     (dbltocmplx(2.0, 0.),
                      (dcmplxadd
                       (dbltocmplx(1.0, 0.),
                        dcmplxmul(dbltocmplx(2.0e-5, 0.), cs)))));
        cf1 =
            dcmplxdiv((dcmplxadd
                      (dbltocmplx(1.0, 0.),
                       dcmplxmul(dbltocmplx(1.7e-4, 0.), cs))),
                     (dcmplxadd
                      (dcmplxadd
                       (dcmplxadd
                        (dbltocmplx(1.0, 0.),
                         dcmplxmul(dbltocmplx(1.038e-3, 0.), cs)),
                        dcmplxmul(dbltocmplx(1.47e-7, 0.),
                                 dcmplxpow(cs, (double) 2))),
                       dcmplxmul(dbltocmplx(1.7e-12, 0.),
                                dcmplxpow(cs, (double) 3)))));
        cf2 = dcmplxmul(dbltocmplx(3.0e-3, 0.), cs);
        cha = dcmplxmul(dcmplxmul(dcmplxmul(cht, chhp), cha1), cha2);
        chb = dcmplxmul(cf1, cf2);
        chs3 =
            dcmplxmul(dcmplxmul
                     (dcmplxmul
                      (dcmplxmul(dbltocmplx(100.0, 0.), cs),
                       (dcmplxdiv
                        (cha,
                         (dcmplxsub(dbltocmplx(1.0, 0.), dcmplxmul(cha, chb)))))),
                      dbltocmplx(1.0e-9, 0.)), cs);
        Xre[i] = dcmplxtof(chs3);
        Xim[i] = daimag(chs3);
    }

    return;
}                               /* end of function */
