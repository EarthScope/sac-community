
#include "icm.h"


void /*FUNCTION*/
gsref(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    double const_;
    complexd pole[11], zero[5];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

    /*   .....USGS Refraction Seismometer.....
     *
     *
     *   .....Set poles and zeros (due to J. Zucca).....
     * */
    const_ = 276.46 * 276.46 * 283.177 * 293.349 * 293.349 * 330.873 * 330.873;
    nzero = 5;

    for (i = 1; i <= nzero; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }

    npole = 11;
    Pole[1] = dcmplxneg(dbltocmplx(10.0531, 7.5398));
    Pole[2] = dcmplxneg(dbltocmplx(10.0531, -7.5398));
    Pole[3] = dcmplxneg(dbltocmplx(0.5969, 0.0));
    Pole[4] = dcmplxneg(dbltocmplx(0.5969, 0.0));
    Pole[5] = dcmplxneg(dbltocmplx(276.4602, 0.0));
    Pole[6] = dcmplxneg(dbltocmplx(276.4602, 0.0));
    Pole[7] = dcmplxneg(dbltocmplx(283.1769, 0.0));
    Pole[8] = dcmplxneg(dbltocmplx(260.2009, 135.4598));
    Pole[9] = dcmplxneg(dbltocmplx(260.2009, -135.4598));
    Pole[10] = dcmplxneg(dbltocmplx(180.6564, 277.2001));
    Pole[11] = dcmplxneg(dbltocmplx(180.6564, -277.2001));

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
