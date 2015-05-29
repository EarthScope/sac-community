
#include "icm.h"
#include "complex_sac.h"

void
dwwssn(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    float const_;
    /*   .....DWWSSN - For a digital WWSSN system response.....
     *            ( poles and zeros due to H. Patton )
     *
     *     digital WWSSN instrumental response.....
     *     default response computed assuming poles and zeros for
     *     stations LON and JAS supplied on day tapes for day 
     *     301, 1983
     * */

    /*   .....Set poles and zeros.....
     * */
    complex double pole[11];
    complex double zero[5];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    const_ = 0.0243 * 500.;
    nzero = 5;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 11;
    Pole[1] = (-0.369) + (.199 * I);
    Pole[2] = (-0.369) + ((-.199) * I);
    Pole[3] = (-0.628) + (0.0 * I);
    Pole[4] = (-0.0209) + (0.0 * I);
    Pole[5] = (-0.0209) + (0.0 * I);
    for (i = 6; i <= npole; i++)
    {
        Pole[i] = (-.273) + (0.0 * I);
    }

    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    return;
}

