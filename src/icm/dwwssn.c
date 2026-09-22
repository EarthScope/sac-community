
#include "icm.h"


void /*FUNCTION*/
dwwssn(int nfreq, double delfrq, double xre[], double xim[])
{
    int i, npole, nzero;
    double const_;
    complexd pole[11], zero[5];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

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
    const_ = 0.0243 * 500.;
    nzero = 5;
    for (i = 1; i <= nzero; i++) {
        Zero[i] = dbltocmplx(0.0, 0.0);
    }

    npole = 11;
    Pole[1] = dbltocmplx(-0.369, .199);
    Pole[2] = dbltocmplx(-0.369, -.199);
    Pole[3] = dbltocmplx(-0.628, 0.0);
    Pole[4] = dbltocmplx(-0.0209, 0.0);
    Pole[5] = dbltocmplx(-0.0209, 0.0);
    for (i = 6; i <= npole; i++) {
        Pole[i] = dbltocmplx(-.273, 0.0);
    }

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

    return;
}                               /* end of function */
