
#include <string.h>

#include "icm.h"
#include "complex_sac.h"

#include "msg.h"

void
sandia(int nfreq, double delfrq, double xre[], double xim[], char *subtyp, int subtyp_s, int *nerr)
{
    int i, npole, nzero;
    float const_, facnew, s1, s2, srf;
    complex double pole[7];
    complex double zero[5];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    /*   .....Sandia system 23 instrumental response.....
     * */

    /*                           <* normalizes max (velocity) of Amplitude */
    facnew = 10.37643443e1;
    const_ = 2.0e4 * facnew;
    nzero = 5;
    /*                           <* Amplifier Transfer Function */
    Zero[1] = (-189.50087) + (0.0 * I);
    for (i = 2; i <= nzero; i++)
    {
        Zero[i] = 0.0 + (0.0 * I);
    }

    npole = 7;
    /*  poles 1 & 2 are amplifier transfer function */
    Pole[1] = (-0.326726) + (0.0 * I);
    Pole[2] = (-0.326726) + (0.0 * I);
    /*  poles 3 to 6 are filter trandfer function */
    Pole[3] = (-125.66371) + (0.0 * I);
    Pole[4] = (-62.83185) + ((-108.82477) * I);
    Pole[5] = (-62.83185) + (108.82477 * I);

    if (subtyp[0] == 'O') {
        if (subtyp[1] == 'L') {
            srf = 1.22;
        } else if (subtyp[1] == 'B') {
            srf = 1.26;
        } else if (subtyp[1] == 'D' || subtyp[1] == 'T') {
            srf = 1.27;
        } else if (subtyp[1] == 'N' || subtyp[1] == 'E') {
            srf = 1.23;
        } else {
            *nerr = 2105;
            goto L_8888;
        }

    } else if (subtyp[0] == 'N') {
        if (memcmp(subtyp + 1, "NV", 2) == 0) {
            srf = 1.29;
        } else
            if ((memcmp(subtyp + 1, "TV", 2) == 0 ||
                 memcmp(subtyp + 1, "BT", 2) == 0) ||
                memcmp(subtyp + 1, "DV", 2) == 0) {
            srf = 1.27;
        } else
            if ((memcmp(subtyp + 1, "TR", 2) == 0 ||
                 memcmp(subtyp + 1, "LT", 2) == 0) ||
                memcmp(subtyp + 1, "DR", 2) == 0) {
            srf = 1.26;
        } else
            if ((memcmp(subtyp + 1, "LR", 2) == 0 ||
                 memcmp(subtyp + 1, "DT", 2) == 0) ||
                memcmp(subtyp + 1, "NR", 2) == 0) {
            srf = 1.25;
        } else
            if ((memcmp(subtyp + 1, "BV", 2) == 0 ||
                 memcmp(subtyp + 1, "BR", 2) == 0) ||
                memcmp(subtyp + 1, "NT", 2) == 0) {
            srf = 1.23;
        } else if (memcmp(subtyp + 1, "TT", 2) == 0 ||
                   memcmp(subtyp + 1, "LV", 2) == 0) {
            srf = 1.22;
        } else {
            *nerr = 2105;
            goto L_8888;
        }
    } else {
        *nerr = 2105;
        goto L_8888;
    }

    /*   .....Allow for variable Seismometer Resonant Frequency ( srf ).....
     *
     *              2                            2
     *           (s)  + 2h(2Pi*SRF)*s + (2pi*SRF)  = 0.0
     *
     *        where:
     *              s = i * omega    and  h = seismometer damping factor
     * */

    s1 = (-150.53419) * srf;
    s2 = (-0.2622555) * srf;

    Pole[6] = s1 + (0.0 * I);
    Pole[7] = s2 + (0.0 * I);
    /*   .....Compute transfer function.....
     * */
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);
    L_8888:
    return;

}

