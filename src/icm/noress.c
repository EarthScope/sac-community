
#include <string.h>

#include "icm.h"
#include "complex_sac.h"

#include "msg.h"

void
noress(int nfreq, double delfrq, double xre[], double xim[], char *subtyp, int subtyp_s)
{
    int idx, nerr;
    float delomg, gkip, gklp, go, omega;
    double complex ch, ch1, ch2, ch3, ch4, ch5, cha, chb, chbp1, chbp2, chc, chd,
        chdf, che, chf, chg, chh, chhp1, chi, chif, chipf, chka, chkv, chlpf,
        cho, chpa, chs3, chsp, cht, ckb, cs;
    static double twopi = 6.283185307179586;
    /*   .....NORESS - for NORESS instrument....
     *          (poles and zeros due to H. Durham)
     * */

    double * const Xim = (&xim[0]) - 1;
    double * const Xre = (&xre[0]) - 1;
    delomg = twopi * delfrq;

    /*   .....computation based on LP, IP, or SP sub-type.....
     * */
    if (memcmp(subtyp, "LP      ", 8) == 0) {
        for (idx = 1; idx <= nfreq; idx++) {
            omega = (float) (idx - 1) * delomg;
            cs = 0.0 + (omega * I);
            cht = (12.66 + (0. * I)) / (((1.0 + (0. * I)) + ((0.02256 + (0. * I)) * cs)) + ((0.6333 + (0. * I)) * cpow(cs, (double) 2)));
            cha = -((3360. + (0. * I)) / ((1.0 + (0. * I)) + ((0.003789 + (0. * I)) * cs)));
            chb = ((1.4215e-4 + (0. * I)) * ((1.0 + (0. * I)) + ((0.01393 + (0. * I)) * cs))) / ((1.0 + (0. * I)) + ((0.0009936 + (0. * I)) * cs));
            ckb = 1370. + (0. * I);
            chka = (cht * cha) / ((1. + (0. * I)) - (((cht * cha) * chb) * ckb));
            chkv = ((100.0 + (0. * I)) * chka) * cs;
            chhp1 = -(((390.6 + (0. * I)) * cs) / (((1.0 + (0. * I)) + ((7.813 + (0. * I)) * cs)) + ((0.4505 + (0. * I)) * cpow(cs, (double) 2))));
            chbp1 = ((324.0 + (0. * I)) * cs) / (((1.0 + (0. * I)) + ((16.90 + (0. * I)) * cs)) + ((113.3 + (0. * I)) * cpow(cs, (double) 2)));
            chdf = -((1.0 + (0. * I)) / ((1.0 + (0. * I)) + ((0.001592 + (0. * I)) * cs)));
            cha = (1.0 + (0. * I)) / ((1.0 + (0. * I)) + ((0.00159 + (0. * I)) * cs));
            chb = (1.0 + (0. * I)) / (((1.0 + (0. * I)) + ((4.576 + (0. * I)) * cs)) + ((11.66 + (0. * I)) * cpow(cs, (double) 2)));
            chc = cs / (((1.0 + (0. * I)) + ((((double) 1504) + (0. * I)) * cs)) * ((1.0 + (0. * I)) + ((32.02 + (0. * I)) * cs)));
            che = (1.0 + (0. * I)) / (((1.0 + (0. * I)) + ((0.9477 + (0. * I)) * cs)) + ((0.2428 + (0. * I)) * cpow(cs, (double) 2)));
            chf = (1.0 + (0. * I)) / (((1.0 + (0. * I)) + ((0.6927 + (0. * I)) * cs)) + ((0.2399 + (0. * I)) * cpow(cs, (double) 2)));
            chg = (1.0 + (0. * I)) / (((1.0 + (0. * I)) + ((0.1913 + (0. * I)) * cs)) + ((0.2403 + (0. * I)) * cpow(cs, (double) 2)));
            chh = cs / (((1.0 + (0. * I)) + ((0.1913 + (0. * I)) * cs)) * ((1.0 + (0. * I)) + ((32.02 + (0. * I)) * cs)));
            gklp = 6589.6;
            chlpf = -(((((((((gklp + (0. * I)) * cha) * chb) * chc) * chb) * che) * chf) * chg) * chh);
            ch = (((((chkv * chhp1) * chbp1) * chdf) * chlpf) * cs) * (1.0e-9 + (0. * I));
            Xre[idx] = creal(ch);
            Xim[idx] = cimag(ch);
        }

    } else if (memcmp(subtyp, "IP      ", 8) == 0) {
        for (idx = 1; idx <= nfreq; idx++) {
            omega = (float) (idx - 1) * delomg;
            cs = 0.0 + omega * I;
            cht = (12.66 + (0. * I)) / (((1.0 + (0. * I)) + ((0.02256 + (0. * I)) * cs)) + ((0.6333 + (0. * I)) * cpow(cs, (double) 2)));
            cha = -((3360. + (0. * I)) / ((1.0 + (0. * I)) + ((0.003789 + (0. * I)) * cs)));
            chb = ((1.4215e-4 + (0. * I)) * ((1.0 + (0. * I)) + ((0.01393 + (0. * I)) * cs))) / ((1.0 + (0. * I)) + ((0.0009936 + (0. * I)) * cs));
            ckb = 1370. + (0. * I);
            chka = (cht * cha) / ((1. + (0. * I)) - (((cht * cha) * chb) * ckb));
            chkv = ((100.0 + (0. * I)) * chka) * cs;
            chbp2 = -((((10.17 + (0. * I)) * cs) * ((127.8 + (0. * I)) + cs)) / (((1.0 + (0. * I)) + ((16.90 + (0. * I)) * cs)) + ((113.1 + (0. * I)) * cpow(cs, (double) 2))));
            chdf = -((1.0 + (0. * I)) / ((1.0 + (0. * I)) + ((0.001592 + (0. * I)) * cs)));
            cha = (1.0 + (0. * I)) / ((1.0 + (0. * I)) + ((0.00159 + (0. * I)) * cs));
            chb = (1.0 + (0. * I)) / (((1.0 + (0. * I)) + ((0.195 + (0. * I)) * cs)) + ((0.00990 + (0. * I)) * cpow(cs, (double) 2)));
            chc = cs / (((1.0 + (0. * I)) + ((0.00153 + (0. * I)) * cs)) * ((1.0 + (0. * I)) + ((5.11 + (0. * I)) * cs)));
            chd = (1.0 + (0. * I)) / (((1.0 + (0. * I)) + ((0.154 + (0. * I)) * cs)) + ((0.00888 + (0. * I)) * cpow(cs, (double) 2)));
            che = (1.0 + (0. * I)) / (((1.0 + (0. * I)) + ((0.0817 + (0. * I)) * cs)) + ((0.0069 + (0. * I)) * cpow(cs, (double) 2)));
            chf = cs / (((1.0 + (0. * I)) + ((0.00419 + (0. * I)) * cs)) * ((1.0 + (0. * I)) + ((5.11 + (0. * I)) * cs)));
            gkip = 129.5;
            chipf = -(((((((gkip + (0. * I)) * cha) * chb) * chc) * chd) * che) * chf);
            ch = ((((chkv * chbp2) * chdf) * chipf) * cs) * (1.0e-9 + (0. * I));
            Xre[idx] = creal(ch);
            Xim[idx] = cimag(ch);
        }
    }
    else if (memcmp(subtyp, "SP      ", 8) == 0) {
        for (idx = 1; idx <= nfreq; idx++) {
            omega = ((float) (idx - 1)) * delomg;
            cs = 0.0 + (omega * I);
            chs3 = ((55.73 + (0. * I)) * cpow(cs, (double) 2)) / (((1.0 + (0. * I)) + ((0.2387 + (0. * I)) * cs)) + ((0.02533 + (0. * I)) * cpow(cs, (double) 2)));
            chif = (0.9174 + (0. * I)) / (((1.0 + (0. * I)) + ((8.165e-4 + (0. * I)) * cs)) + ((7.339e-7 + (0. * I)) * cpow(cs, (double) 2)));
            chpa = ((5.544 + (0. * I)) * cs) / (((1.0 + (0. * I)) + ((1.559e-3 + (0. * I)) * cs)) * ((1.0 + (0. * I)) + ((0.1598 + (0. * I)) * cs)));
            go = 50.0;
            chi = (1.0 + (0. * I)) / ((1.0 + (0. * I)) + ((7.950e-4 + (0. * I)) * cs));
            ch1 = (1.0 + (0. * I)) / (((1.0 + (0. * I)) + ((0.0221 + (0. * I)) * cs)) + ((0.000125 + (0. * I)) * cpow(cs, (double) 2)));
            ch2 = (1.0 + (0. * I)) / (((1.0 + (0. * I)) + ((0.0195 + (0. * I)) * cs)) + ((0.000119 + (0. * I)) * cpow(cs, (double) 2)));
            ch3 = (1.0 + (0. * I)) / (((1.0 + (0. * I)) + ((0.0143 + (0. * I)) * cs)) + ((0.000104 + (0. * I)) * cpow(cs, (double) 2)));
            ch4 = (1.0 + (0. * I)) / (((1.0 + (0. * I)) + ((0.00739 + (0. * I)) * cs)) + ((0.0000820 + (0. * I)) * cpow(cs, (double) 2)));
            ch5 = (1.0 + (0. * I)) / (((1.0 + (0. * I)) + ((0.0150 + (0. * I)) * cs)) + ((0.000113 + (0. * I)) * cpow(cs, (double) 2)));
            cho = ((0.03186 + (0. * I)) * cs) / (((1.0 + (0. * I)) + ((0.1593 + (0. * I)) * cs)) * ((1.0 + (0. * I)) + ((1.416e-3 + (0. * I)) * cs)));
            chsp = -((((((((go + (0. * I)) * chi) * ch1) * ch2) * ch3) * ch4) * ch5) * cho);
            chdf = -((1.0 + (0. * I)) / ((1.0 + (0. * I)) + ((0.001592 + (0. * I)) * cs)));
            ch = (((((chs3 * chif) * chpa) * chsp) * chdf) * cs) * (1.0e-9 + (0. * I));
            Xre[idx] = creal(ch);
            Xim[idx] = cimag(ch);
        }
    } else {
        nerr = 2105;
        return;
    }



}

