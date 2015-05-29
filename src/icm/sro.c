
#include <string.h>

#include "icm.h"
#include "complex_sac.h"

#include "msg.h"

void
sro(int nfreq, double delfrq, double xre[], double xim[], char *subtyp, int subtyp_s)
{
    int nerr, npole, nzero;
    float const_;

    complex double pole[17];
    complex double zero[8];
    complex double * const Pole = (&pole[0]) - 1;
    complex double * const Zero = (&zero[0]) - 1;
    /*   .....SRO - for an SRO seismometer.....
     * */

    /*   .....Set poles and zeros.....
     *
     *
     * SRO SEISMOMETER.  THERE ARE THREE SYSTEMS THAT CAN BE COMPUTED
     * HERE:  BROADBAND (BB), SHORT PERIOD (SP), OR LONG PERIOD (LP).
     * REFERENCES ARE:
     * MCCOWAN, D. W. AND R. T. LACOSS (1978).  TRANSFER FUNCTION
     * FOR THE SEISMIC RESEARCH OBSERVATORY SEISMOGRAPH SYSTEM, BULL. SEIS.
     * SOC. AM., 68, 501-512.
     * BERGER, J., D. W. MCCOWAN, W. E. FARRELL, AND R. T. LACOSS (1978).
     * COMMENTS ON 'TRANSFER FUNCTIONS FOR THE SEISMIC RESEARCH OBSERVATORY
     * SEISMOGRAPH SYSTEM' BY DOUGLAS W. MCCOWAN AND RICHARD T. LACOSS, BULL.
     * SEIS. SOC. AM., 68, 1537-1538.
     *
     *
     *  .....SRO BROADBAND     'BB'.....
     * */
    if (memcmp(subtyp, "BB", 2) == 0) {
        const_ = -394.0;

        nzero = 4;
        Zero[1] = (-0.125) + (0.0 * I);
        Zero[2] = (-50.0) + (0.0 * I);
        Zero[3] = 0.0 + (0.0 * I);
        Zero[4] = 0.0 + (0.0 * I);

        npole = 4;
        Pole[1] = (-0.13) + (0.0 * I);
        Pole[2] = (-6.02) + (0.0 * I);
        Pole[3] = (-8.66) + (0.0 * I);
        Pole[4] = (-35.2) + (0.0 * I);

        /*   .....SRO SHORT PERIOD.      'SP'.....
         * INCLUDES SEISMOMETER AND SHAPING FILTER TRANSFER FUNCTIONS.
         * */
    } else if (memcmp(subtyp, "SP", 2) == 0) {
        const_ = 5.08233208e13;

        nzero = 5;
        Zero[1] = (-50.0) + (0.0 * I);
        Zero[2] = 0.0 + (0.0 * I);
        Zero[3] = 0.0 + (0.0 * I);
        Zero[4] = 0.0 + (0.0 * I);
        Zero[5] = 0.0 + (0.0 * I);

        npole = 9;
        Pole[1] = (-0.13) + (0.0 * I);
        Pole[2] = (-6.02) + (0.0 * I);
        Pole[3] = (-8.66) + (0.0 * I);
        Pole[4] = (-35.2) + (0.0 * I);
        Pole[5] = (-100.0) + (0.0 * I);
        Pole[6] = (-17.97) + (0.0 * I);
        Pole[7] = (-17.97) + (0.0 * I);
        Pole[8] = (-63.29) + (0.0 * I);
        Pole[9] = (-63.29) + (0.0 * I);
        /*   .....SRO LONG PERIOD         'LP'.....
         * INCLUDES SEISMOMETER, SHAPING FILTERS, AND ANTI-ALIASING FILTERS.
         * */
    } else if (memcmp(subtyp, "LPDE", 4) == 0) {
        const_ = 16892.61226;

        nzero = 8;
        Zero[1] = -50.0 + 0.0 * I;
        Zero[2] = 0.0 + 0.0 * I;
        Zero[3] = 0.0 + 0.0 * I;
        Zero[4] = 0.0 + 0.0 * I;
        Zero[5] = 0.0 + 1.05 * I;
        Zero[6] = 0.0 + -1.05 * I;
        Zero[7] = 0.0 + 0.0 * I;
        Zero[8] = 0.0 + 0.0 * I;

        npole = 17;
        Pole[1] = -0.13 + 0.0 * I;
        Pole[2] = -6.02 + 0.0 * I;
        Pole[3] = -8.66 + 0.0 * I;
        Pole[4] = -35.2 + 0.0 * I;
        Pole[5] = -100.0 + 0.0 * I;
        Pole[6] = -3.93 + 0.0 * I;
        Pole[7] = -0.282 + 0.0 * I;
        Pole[8] = -0.201 + 0.241 * I;
        Pole[9] = -0.201 + -0.241 * I;
        Pole[10] = -0.134 + 0.1 * I;
        Pole[11] = -0.134 + -0.1 * I;
        Pole[12] = -0.0251 + 0.0 * I;
        Pole[13] = -0.00942 + 0.0 * I;
        Pole[14] = -0.24 + 0.58 * I;
        Pole[15] = -0.58 + 0.24 * I;
        Pole[16] = -0.58 + -0.24 * I;
        Pole[17] = -0.24 + -0.58 * I;

    } else {
        nerr = 2105;
        goto L_8888;
    }

    /*   .....Compute transfer function.....
     * */
    getranx(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

  L_8888:
    return;
}                               /* end of function */
