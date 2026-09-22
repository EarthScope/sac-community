
#include <string.h>

#include "icm.h"


#include "msg.h"

void /*FUNCTION*/
noress(int nfreq, double delfrq, double xre[], double xim[], char *subtyp,
       int subtyp_s)
{
    int idx, nerr;
    double delomg, gkip, gklp, go, omega;
    complexd ch, ch1, ch2, ch3, ch4, ch5, cha, chb, chbp1, chbp2, chc, chd,
        chdf, che, chf, chg, chh, chhp1, chi, chif, chipf, chka, chkv, chlpf,
        cho, chpa, chs3, chsp, cht, ckb, cs;
    static double twopi = 6.283185307179586;

    double *const Xim = &xim[0] - 1;
    double *const Xre = &xre[0] - 1;

    /*   .....NORESS - for NORESS instrument....
     *          (poles and zeros due to H. Durham)
     * */

    delomg = twopi * delfrq;

    /*   .....computation based on LP, IP, or SP sub-type.....
     * */
    if (memcmp(subtyp, "LP      ", 8) == 0) {
        for (idx = 1; idx <= nfreq; idx++) {
            omega = (double) (idx - 1) * delomg;
            cs = dbltocmplx(0.0, omega);
            cht =
                dcmplxdiv(dbltocmplx(12.66, 0.),
                         (dcmplxadd
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.02256, 0.), cs)),
                           dcmplxmul(dbltocmplx(0.6333, 0.),
                                    dcmplxpow(cs, (double) 2)))));
            cha =
                dcmplxneg(dcmplxdiv
                         (dbltocmplx(3360., 0.),
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.003789, 0.), cs)))));
            chb =
                dcmplxdiv(dcmplxmul
                         (dbltocmplx(1.4215e-4, 0.),
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.01393, 0.), cs)))),
                         (dcmplxadd
                          (dbltocmplx(1.0, 0.),
                           dcmplxmul(dbltocmplx(0.0009936, 0.), cs))));
            ckb = dbltocmplx(1370., 0.);
            chka =
                dcmplxdiv(dcmplxmul(cht, cha),
                         (dcmplxsub
                          (dbltocmplx(1., 0.),
                           dcmplxmul(dcmplxmul(dcmplxmul(cht, cha), chb), ckb))));
            chkv = dcmplxmul(dcmplxmul(dbltocmplx(100.0, 0.), chka), cs);
            chhp1 =
                dcmplxneg(dcmplxdiv
                         (dcmplxmul(dbltocmplx(390.6, 0.), cs),
                          (dcmplxadd
                           (dcmplxadd
                            (dbltocmplx(1.0, 0.),
                             dcmplxmul(dbltocmplx(7.813, 0.), cs)),
                            dcmplxmul(dbltocmplx(0.4505, 0.),
                                     dcmplxpow(cs, (double) 2))))));
            chbp1 =
                dcmplxdiv(dcmplxmul(dbltocmplx(324.0, 0.), cs),
                         (dcmplxadd
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(16.90, 0.), cs)),
                           dcmplxmul(dbltocmplx(113.3, 0.),
                                    dcmplxpow(cs, (double) 2)))));
            chdf =
                dcmplxneg(dcmplxdiv
                         (dbltocmplx(1.0, 0.),
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.001592, 0.), cs)))));
            cha =
                dcmplxdiv(dbltocmplx(1.0, 0.),
                         (dcmplxadd
                          (dbltocmplx(1.0, 0.),
                           dcmplxmul(dbltocmplx(0.00159, 0.), cs))));
            chb =
                dcmplxdiv(dbltocmplx(1.0, 0.),
                         (dcmplxadd
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(4.576, 0.), cs)),
                           dcmplxmul(dbltocmplx(11.66, 0.),
                                    dcmplxpow(cs, (double) 2)))));
            chc =
                dcmplxdiv(cs,
                         (dcmplxmul
                          ((dcmplxadd
                            (dbltocmplx(1.0, 0.),
                             dcmplxmul(dbltocmplx((double) 1504, 0.), cs))),
                           (dcmplxadd
                            (dbltocmplx(1.0, 0.),
                             dcmplxmul(dbltocmplx(32.02, 0.), cs))))));
            che =
                dcmplxdiv(dbltocmplx(1.0, 0.),
                         (dcmplxadd
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.9477, 0.), cs)),
                           dcmplxmul(dbltocmplx(0.2428, 0.),
                                    dcmplxpow(cs, (double) 2)))));
            chf =
                dcmplxdiv(dbltocmplx(1.0, 0.),
                         (dcmplxadd
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.6927, 0.), cs)),
                           dcmplxmul(dbltocmplx(0.2399, 0.),
                                    dcmplxpow(cs, (double) 2)))));
            chg =
                dcmplxdiv(dbltocmplx(1.0, 0.),
                         (dcmplxadd
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.1913, 0.), cs)),
                           dcmplxmul(dbltocmplx(0.2403, 0.),
                                    dcmplxpow(cs, (double) 2)))));
            chh =
                dcmplxdiv(cs,
                         (dcmplxmul
                          ((dcmplxadd
                            (dbltocmplx(1.0, 0.),
                             dcmplxmul(dbltocmplx(0.1913, 0.), cs))),
                           (dcmplxadd
                            (dbltocmplx(1.0, 0.),
                             dcmplxmul(dbltocmplx(32.02, 0.), cs))))));
            gklp = 6589.6;
            chlpf =
                dcmplxneg(dcmplxmul
                         (dcmplxmul
                          (dcmplxmul
                           (dcmplxmul
                            (dcmplxmul
                             (dcmplxmul
                              (dcmplxmul
                               (dcmplxmul(dbltocmplx(gklp, 0.), cha), chb), chc),
                              chb), che), chf), chg), chh));
            ch = dcmplxmul(dcmplxmul
                          (dcmplxmul
                           (dcmplxmul
                            (dcmplxmul(dcmplxmul(chkv, chhp1), chbp1), chdf),
                            chlpf), cs), dbltocmplx(1.0e-9, 0.));
            Xre[idx] = dcmplxtof(ch);
            Xim[idx] = daimag(ch);
        }
        return;

    } else if (memcmp(subtyp, "IP      ", 8) == 0) {
        for (idx = 1; idx <= nfreq; idx++) {
            omega = (double) (idx - 1) * delomg;
            cs = dbltocmplx(0.0, omega);
            cht =
                dcmplxdiv(dbltocmplx(12.66, 0.),
                         (dcmplxadd
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.02256, 0.), cs)),
                           dcmplxmul(dbltocmplx(0.6333, 0.),
                                    dcmplxpow(cs, (double) 2)))));
            cha =
                dcmplxneg(dcmplxdiv
                         (dbltocmplx(3360., 0.),
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.003789, 0.), cs)))));
            chb =
                dcmplxdiv(dcmplxmul
                         (dbltocmplx(1.4215e-4, 0.),
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.01393, 0.), cs)))),
                         (dcmplxadd
                          (dbltocmplx(1.0, 0.),
                           dcmplxmul(dbltocmplx(0.0009936, 0.), cs))));
            ckb = dbltocmplx(1370., 0.);
            chka =
                dcmplxdiv(dcmplxmul(cht, cha),
                         (dcmplxsub
                          (dbltocmplx(1., 0.),
                           dcmplxmul(dcmplxmul(dcmplxmul(cht, cha), chb), ckb))));
            chkv = dcmplxmul(dcmplxmul(dbltocmplx(100.0, 0.), chka), cs);
            chbp2 =
                dcmplxneg(dcmplxdiv
                         (dcmplxmul
                          (dcmplxmul(dbltocmplx(10.17, 0.), cs),
                           (dcmplxadd(dbltocmplx(127.8, 0.), cs))),
                          (dcmplxadd
                           (dcmplxadd
                            (dbltocmplx(1.0, 0.),
                             dcmplxmul(dbltocmplx(16.90, 0.), cs)),
                            dcmplxmul(dbltocmplx(113.1, 0.),
                                     dcmplxpow(cs, (double) 2))))));
            chdf =
                dcmplxneg(dcmplxdiv
                         (dbltocmplx(1.0, 0.),
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.001592, 0.), cs)))));
            cha =
                dcmplxdiv(dbltocmplx(1.0, 0.),
                         (dcmplxadd
                          (dbltocmplx(1.0, 0.),
                           dcmplxmul(dbltocmplx(0.00159, 0.), cs))));
            chb =
                dcmplxdiv(dbltocmplx(1.0, 0.),
                         (dcmplxadd
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.195, 0.), cs)),
                           dcmplxmul(dbltocmplx(0.00990, 0.),
                                    dcmplxpow(cs, (double) 2)))));
            chc =
                dcmplxdiv(cs,
                         (dcmplxmul
                          ((dcmplxadd
                            (dbltocmplx(1.0, 0.),
                             dcmplxmul(dbltocmplx(0.00153, 0.), cs))),
                           (dcmplxadd
                            (dbltocmplx(1.0, 0.),
                             dcmplxmul(dbltocmplx(5.11, 0.), cs))))));
            chd =
                dcmplxdiv(dbltocmplx(1.0, 0.),
                         (dcmplxadd
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.154, 0.), cs)),
                           dcmplxmul(dbltocmplx(0.00888, 0.),
                                    dcmplxpow(cs, (double) 2)))));
            che =
                dcmplxdiv(dbltocmplx(1.0, 0.),
                         (dcmplxadd
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.0817, 0.), cs)),
                           dcmplxmul(dbltocmplx(0.0069, 0.),
                                    dcmplxpow(cs, (double) 2)))));
            chf =
                dcmplxdiv(cs,
                         (dcmplxmul
                          ((dcmplxadd
                            (dbltocmplx(1.0, 0.),
                             dcmplxmul(dbltocmplx(0.00419, 0.), cs))),
                           (dcmplxadd
                            (dbltocmplx(1.0, 0.),
                             dcmplxmul(dbltocmplx(5.11, 0.), cs))))));
            gkip = 129.5;
            chipf =
                dcmplxneg(dcmplxmul
                         (dcmplxmul
                          (dcmplxmul
                           (dcmplxmul
                            (dcmplxmul(dcmplxmul(dbltocmplx(gkip, 0.), cha), chb),
                             chc), chd), che), chf));
            ch = dcmplxmul(dcmplxmul
                          (dcmplxmul
                           (dcmplxmul(dcmplxmul(chkv, chbp2), chdf), chipf), cs),
                          dbltocmplx(1.0e-9, 0.));
            Xre[idx] = dcmplxtof(ch);
            Xim[idx] = daimag(ch);
        }
        return;

    } else if (memcmp(subtyp, "SP      ", 8) == 0) {
        for (idx = 1; idx <= nfreq; idx++) {
            omega = (double) (idx - 1) * delomg;
            cs = dbltocmplx(0.0, omega);
            chs3 =
                dcmplxdiv(dcmplxmul
                         (dbltocmplx(55.73, 0.), dcmplxpow(cs, (double) 2)),
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
            go = 50.0;
            chi =
                dcmplxdiv(dbltocmplx(1.0, 0.),
                         (dcmplxadd
                          (dbltocmplx(1.0, 0.),
                           dcmplxmul(dbltocmplx(7.950e-4, 0.), cs))));
            ch1 =
                dcmplxdiv(dbltocmplx(1.0, 0.),
                         (dcmplxadd
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.0221, 0.), cs)),
                           dcmplxmul(dbltocmplx(0.000125, 0.),
                                    dcmplxpow(cs, (double) 2)))));
            ch2 =
                dcmplxdiv(dbltocmplx(1.0, 0.),
                         (dcmplxadd
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.0195, 0.), cs)),
                           dcmplxmul(dbltocmplx(0.000119, 0.),
                                    dcmplxpow(cs, (double) 2)))));
            ch3 =
                dcmplxdiv(dbltocmplx(1.0, 0.),
                         (dcmplxadd
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.0143, 0.), cs)),
                           dcmplxmul(dbltocmplx(0.000104, 0.),
                                    dcmplxpow(cs, (double) 2)))));
            ch4 =
                dcmplxdiv(dbltocmplx(1.0, 0.),
                         (dcmplxadd
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.00739, 0.), cs)),
                           dcmplxmul(dbltocmplx(0.0000820, 0.),
                                    dcmplxpow(cs, (double) 2)))));
            ch5 =
                dcmplxdiv(dbltocmplx(1.0, 0.),
                         (dcmplxadd
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.0150, 0.), cs)),
                           dcmplxmul(dbltocmplx(0.000113, 0.),
                                    dcmplxpow(cs, (double) 2)))));
            cho =
                dcmplxdiv(dcmplxmul(dbltocmplx(0.03186, 0.), cs),
                         (dcmplxmul
                          ((dcmplxadd
                            (dbltocmplx(1.0, 0.),
                             dcmplxmul(dbltocmplx(0.1593, 0.), cs))),
                           (dcmplxadd
                            (dbltocmplx(1.0, 0.),
                             dcmplxmul(dbltocmplx(1.416e-3, 0.), cs))))));
            chsp =
                dcmplxneg(dcmplxmul
                         (dcmplxmul
                          (dcmplxmul
                           (dcmplxmul
                            (dcmplxmul
                             (dcmplxmul(dcmplxmul(dbltocmplx(go, 0.), chi), ch1),
                              ch2), ch3), ch4), ch5), cho));
            chdf =
                dcmplxneg(dcmplxdiv
                         (dbltocmplx(1.0, 0.),
                          (dcmplxadd
                           (dbltocmplx(1.0, 0.),
                            dcmplxmul(dbltocmplx(0.001592, 0.), cs)))));
            ch = dcmplxmul(dcmplxmul
                          (dcmplxmul
                           (dcmplxmul
                            (dcmplxmul(dcmplxmul(chs3, chif), chpa), chsp), chdf),
                           cs), dbltocmplx(1.0e-9, 0.));
            Xre[idx] = dcmplxtof(ch);
            Xim[idx] = daimag(ch);
        }
        return;
    } else {
        nerr = 2105;
        setmsg("ERROR", nerr);
        apcmsg("NORESS:", 8);
        apcmsg(subtyp, subtyp_s);
        return;
    }

}                               /* end of function */
