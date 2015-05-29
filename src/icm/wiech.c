
#include <math.h>

#include "icm.h"
#include "complex_sac.h"
#include "co.h"

void
wiech(int nfreq, double delfrq, double xre[], double xim[])
{
    int npole, nzero;
    float const_, h, om0, rad, t0;
    complex double pole[2];
    complex double zero[2];
    static double twopi = 6.283185307179586;
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;

    /*   .....WIEC - for a Wiechert seismometer.....
     * */

    /* MECHANICAL INSTRUMENT:  AMPLITUDE AND PHASE RESPONSES ARE COMPUTED
     * FROM EQNS. (82), (98), AND (13) OF SOHON (1932), SEISMOMETRY, IN
     * PART II OF INTRODUCTION TO THEORETICAL SEISMOLOGY, J. B. MACELWANE
     * AND F. W. SOHON, JOHN WILEY AND SONS, NEW YORK.
     * */
    t0 = 9.65;
    h = 0.403712752;
    const_ = 188.5;
    om0 = twopi / t0;
    nzero = 2;
    Zero[1] = 0.0 + (0.0 * I);
    Zero[2] = 0.0 + (0.0 * I);

    npole = 2;
    /*  ??????????????????????????????????????? */
    rad = sqrt(1.0 - powi(h, 2));
    Pole[1] = ((-om0) * h) + ((om0 * rad) * I);
    Pole[2] = ((-om0) * h) + (((-om0) * rad) * I);
    /*  ???????????????????????????????????????
     *
     *   .....Compute transfer function.....
     * */
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

