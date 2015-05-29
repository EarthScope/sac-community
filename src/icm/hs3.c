
#include "icm.h"
#include "complex_sac.h"

void
hs3(int nfreq, double delfrq, double xre[], double xim[])
{
    int i;
    float delomg, omega;
    double complex  cf1, cf2, cha, cha1, cha2, chb, chhp, chs3, cht, cs;
    static double twopi = 6.283185307179586;
    /*   .....S-750 Seismometer Response - output is in units of volts/nm....
     * */

    double * const Xim = (&xim[0]) - 1;
    double * const Xre = (&xre[0]) - 1;
    delomg = twopi * delfrq;
    /*   .....Set poles and zeros.....
     * */
    for (i = 1; i <= nfreq; i++) {
        omega = (float) (i - 1) * delomg;
        cs = 0.0 + (omega * I);
        cht = (2.6e3 + (0. * I)) / (((1.11e5 + (0. * I)) + ((33.3 + (0. * I)) * cs)) + cpow(cs, (double) 2));
        chhp = ((16.0 + (0. * I)) * cs) / ((1.0 + (0. * I)) + ((1.0e-4 + (0. * I)) * cs));
        cha1 = ((21.0 + (0. * I)) + ((1.0e-4 + (0. * I)) * cs)) / ((1.0 + (0. * I)) + ((1.0e-4 + (0. * I)) * cs));
        cha2 = -((2.0 + (0. * I)) / ((1.0 + (0. * I)) + ((2.0e-5 + (0. * I)) * cs)));
        cf1 = ((1.0 + (0. * I)) + ((1.7e-4 + (0. * I)) * cs)) / ((((1.0 + (0. * I)) + ((1.038e-3 + (0. * I)) * cs)) + ((1.47e-7 + (0. * I)) * cpow(cs, (double) 2))) + ((1.7e-12 + (0. * I)) * cpow(cs, (double) 3)));
        cf2 = (3.0e-3 + (0. * I)) * cs;
        cha = ((cht * chhp) * cha1) * cha2;
        chb = cf1 * cf2;
        chs3 = ((((100.0 + (0. * I)) * cs) * (cha / ((1.0 + (0. * I)) - (cha * chb)))) * (1.0e-9 + (0. * I))) * cs;
        Xre[i] = creal(chs3);
        Xim[i] = cimag(chs3);
    }

    return;
}

