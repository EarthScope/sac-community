
#include "icm.h"
#include "complex_sac.h"

void
noresshf(int nfreq, double delfrq, double xre[], double xim[])
{
    int i;
    float delomg, omega;
    double complex chhf, chhp, chia, chif, chlp, chpa, chsi, cs, temp1, temp2, temp3;
    static double twopi = 6.283185307179586;
    /*   .....NORESS high frequency element (Durham, 6 March 1986).....
     *          output in volts/nm
     *          A. Smith, 6 May 1986
     * */

    double * const Xim = (&xim[0]) - 1;
    double * const Xre = (&xre[0]) - 1;
    delomg = twopi * delfrq;
    for (i = 1; i <= nfreq; i++) {
        omega = (float) (i - 1) * delomg;
        cs = 0.0 + (omega * I);
        chsi = ((55.73 + (0. * I)) * cpow(cs, (double) 2)) / (((1.0 + (0. * I)) + ((0.2387 + (0. * I)) * cs)) + ((0.02533 + (0. * I)) * cpow(cs, (double) 2)));
        chif = (0.9174 + (0. * I)) / (((1.0 + (0. * I)) + ((8.165e-4 + (0. * I)) * cs)) + ((7.339e-7 + (0. * I)) * cpow(cs, (double) 2)));
        chpa = ((5.544 + (0. * I)) * cs) / (((1.0 + (0. * I)) + ((1.559e-3 + (0. * I)) * cs)) * ((1.0 + (0. * I)) + ((0.1598 + (0. * I)) * cs)));
        chia = ((0.3186 + (0. * I)) * cs) / (((1.0 + (0. * I)) + ((1.593 + (0. * I)) * cs)) * ((1.0 + (0. * I)) + ((2.496e-3 + (0. * I)) * cs)));
        temp1 = cs * cs;
        temp2 = (0.0124 + (0. * I)) * temp1;
        temp3 = ((1.0 + (0. * I)) + ((0.0500 + (0. * I)) * cs)) * ((1.0 + (0. * I)) + ((0.124 + (0. * I)) * cs));
        chhp = temp2 / temp3;
        chlp = (1.0 + (0. * I)) / ((1.0 + (0. * I)) + cpow((2.894e-3 + (0. * I)) * cs, (double) 36));
        chhf = ((((((chsi * chif) * chpa) * chia) * chhp) * chlp) * (1.0e-9 + (0. * I))) * cs;
        Xre[i] = creal(chhf);
        Xim[i] = cimag(chhf);
    }

    return;
}

