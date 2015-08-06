
#include <math.h>
#include <string.h>

#include "icm.h"
#include "co.h"
#include "complex_sac.h"

#include "msg.h"

void
lll(int nfreq, double delfrq, double xre[], double xim[], char *subtyp, int subtyp_s, double freepd, double damp, int *nerr)
{
    int i, npole, nzero;
    float const_, h, om0, rad, t0;
    complex double pole[8];
    complex double zero[3];
    static double twopi = 6.283185307179586;
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    /*   .....LLL - for all LLL broadband (analog) seismometers.....
     * */

    /* LLL STATION NETWORK AT ELKO, KANAB, LANDERS, MINA.
     * THIS SECTION CONSISTS OF THE INSTRUMENT RESPONSE AS WELL AS THE IW
     * TERM TO CONVERT DISPLACEMENT INTO VELOCITY.
     * THE TRANSFER FUNCTION IS SHIFTED BY PI FROM THE PUBLISHED TRANSFER
     * FUNCTION IN ORDER TO CONFORM WITH SIGN CONVENTIONS.
     * REFERENCES ARE:
     *     JUDD, D. D. (1969), DETERMINATION OF TRANSFER FUNCTION OF
     *        SEISMOMETER/TELEMETRY SYSTEM THROUGH USE OF PSEUDO-RANDOM
     *        BINARY SEQUENCES, UCRL-50490.
     *     DENNY, M. D. (1977), THE INSTALLATION OF HORIZONTAL SEISMOMETERS
     *        IN THE LLL SEISMIC NET AND THEIR CALIBRATION, UCRL-52216.
     * */
    if (memcmp(subtyp, "LV", 2) == 0) {
        t0 = 20.9;
        h = 0.800;
    } else if (memcmp(subtyp, "LR", 2) == 0) {
        t0 = 39.2;
        h = 0.707;
    } else if (memcmp(subtyp, "LT", 2) == 0) {
        t0 = 39.6;
        h = 0.707;
    } else if (memcmp(subtyp, "MV", 2) == 0) {
        t0 = 19.3;
        h = 0.790;
    } else if (memcmp(subtyp, "MR", 2) == 0) {
        t0 = 41.5;
        h = 0.707;
    } else if (memcmp(subtyp, "MT", 2) == 0) {
        t0 = 40.6;
        h = 0.707;
    } else if (memcmp(subtyp, "KV", 2) == 0) {
        t0 = 20.0;
        h = .800;
    } else if (memcmp(subtyp, "KR", 2) == 0) {
        t0 = 40.56;
        h = 0.707;
    } else if (memcmp(subtyp, "KT", 2) == 0) {
        t0 = 39.98;
        h = 0.707;
    } else if (memcmp(subtyp, "EV", 2) == 0) {
        t0 = 30.9;
        h = 0.770;
    } else if (memcmp(subtyp, "ER", 2) == 0) {
        t0 = 40.4;
        h = 0.707;
    } else if (memcmp(subtyp, "ET", 2) == 0) {
        t0 = 40.4;
        h = 0.707;
    } else if (memcmp(subtyp, "BB", 2) == 0) {
        t0 = freepd;
        h = damp;
    } else {
        *nerr = 2105;
        goto L_8888;
    }

    om0 = twopi / t0;
    const_ = 3.93785011e12;
    nzero = 3;
    for (i = 1; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 8;
    /*    ??????????????????????????????? */
    rad = sqrt(1.0 - pow(h, 2));
    Pole[1] = ((-om0) * h) + ((om0 * rad) * I);
    Pole[2] = ((-om0) * h) + (((-om0) * rad) * I);
    /* ???????????????????????????????????? */
    Pole[3] = (-114.28) + (23.317 * I);
    Pole[4] = (-114.28) + ((-23.317) * I);
    Pole[5] = (-100.48) + (70.65 * I);
    Pole[6] = (-100.48) + ((-70.65) * I);
    Pole[7] = (-67.677) + (120.85 * I);
    Pole[8] = (-67.677) + ((-120.85) * I);

    /*   .....Compute transfer function.....
     * */
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    L_8888:
    return;

}

