
#include "icm.h"
#include "complex_sac.h"

void
gsref(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    /*   .....USGS Refraction Seismometer.....
     *
     *
     *   .....Set poles and zeros (due to J. Zucca).....
     * */
    const_ = 276.46 * 276.46 * 283.177 * 293.349 * 293.349 * 330.873 * 330.873;
    complex double pole[11];
    complex double zero[5];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    nzero = 5;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 11;
    Pole[1] = -(10.0531 + (7.5398 * I));
    Pole[2] = -(10.0531 + ((-7.5398) * I));
    Pole[3] = -(0.5969 + (0.0 * I));
    Pole[4] = -(0.5969 + (0.0 * I));
    Pole[5] = -(276.4602 + (0.0 * I));
    Pole[6] = -(276.4602 + (0.0 * I));
    Pole[7] = -(283.1769 + (0.0 * I));
    Pole[8] = -(260.2009 + (135.4598 * I));
    Pole[9] = -(260.2009 + ((-135.4598) * I));
    Pole[10] = -(180.6564 + (277.2001 * I));
    Pole[11] = -(180.6564 + ((-277.2001) * I));
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

