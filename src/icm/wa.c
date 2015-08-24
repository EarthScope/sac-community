
#include <math.h>

#include "icm.h"
#include "co.h"

void /*FUNCTION*/
wa(nfreq, delfrq, xre, xim)
     int nfreq;
     double delfrq, xre[], xim[];
{
    int npole, nzero;
    double const_, h, om0, rad, t0;
    complexd pole[2], zero[2];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....WA - for a Wood-Anderson seismometer.....
     * */

    /* MECHANICAL INSTRUMENT:  AMPLITUDE AND PHASE RESPONSES ARE COMPUTED
     * FROM EQNS. (82), (98), AND (13) OF SOHON (1932), SEISMOMETRY, IN
     * PART II OF INTRODUCTION TO THEORETICAL SEISMOLOGY, J. B. MACELWANE
     * AND F. W. SOHON, JOHN WILEY AND SONS, NEW YORK.
     * */
    t0 = 0.8;
    h = 0.8;

    const_ = 2800.0;
    om0 = M_PI * 2 / t0;
    nzero = 2;
    Zero[1] = dbltocmplx(0.0, 0.0);
    Zero[2] = dbltocmplx(0.0, 0.0);

    npole = 2;
    /*  ??????????????????????????????????????? */
    rad = sqrt(1.0 - (h*h));

    Pole[1] = dbltocmplx(-om0 * h, om0 * rad);
    Pole[2] = dbltocmplx(-om0 * h, -om0 * rad);
    /*  ???????????????????????????????????????
     *
     *   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
