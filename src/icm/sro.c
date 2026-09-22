
#include <string.h>

#include "icm.h"


#include "msg.h"

void /*FUNCTION*/
sro(int nfreq, double delfrq, double xre[], double xim[], char *subtyp,
    int subtyp_s)
{
    int nerr, npole, nzero;
    double const_;
    complexd pole[17], zero[8];

    complexd *const Pole = &pole[0] - 1;
    complexd *const Zero = &zero[0] - 1;

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
        Zero[1] = dbltocmplx(-0.125, 0.0);
        Zero[2] = dbltocmplx(-50.0, 0.0);
        Zero[3] = dbltocmplx(0.0, 0.0);
        Zero[4] = dbltocmplx(0.0, 0.0);

        npole = 4;
        Pole[1] = dbltocmplx(-0.13, 0.0);
        Pole[2] = dbltocmplx(-6.02, 0.0);
        Pole[3] = dbltocmplx(-8.66, 0.0);
        Pole[4] = dbltocmplx(-35.2, 0.0);

        /*   .....SRO SHORT PERIOD.      'SP'.....
         * INCLUDES SEISMOMETER AND SHAPING FILTER TRANSFER FUNCTIONS.
         * */
    } else if (memcmp(subtyp, "SP", 2) == 0) {
        const_ = 5.08233208e13;

        nzero = 5;
        Zero[1] = dbltocmplx(-50.0, 0.0);
        Zero[2] = dbltocmplx(0.0, 0.0);
        Zero[3] = dbltocmplx(0.0, 0.0);
        Zero[4] = dbltocmplx(0.0, 0.0);
        Zero[5] = dbltocmplx(0.0, 0.0);

        npole = 9;
        Pole[1] = dbltocmplx(-0.13, 0.0);
        Pole[2] = dbltocmplx(-6.02, 0.0);
        Pole[3] = dbltocmplx(-8.66, 0.0);
        Pole[4] = dbltocmplx(-35.2, 0.0);
        Pole[5] = dbltocmplx(-100.0, 0.0);
        Pole[6] = dbltocmplx(-17.97, 0.0);
        Pole[7] = dbltocmplx(-17.97, 0.0);
        Pole[8] = dbltocmplx(-63.29, 0.0);
        Pole[9] = dbltocmplx(-63.29, 0.0);

        /*   .....SRO LONG PERIOD         'LP'.....
         * INCLUDES SEISMOMETER, SHAPING FILTERS, AND ANTI-ALIASING FILTERS.
         * */
    } else if (memcmp(subtyp, "LPDE", 4) == 0) {
        const_ = 16892.61226;

        nzero = 8;
        Zero[1] = dbltocmplx(-50.0, 0.0);
        Zero[2] = dbltocmplx(0.0, 0.0);
        Zero[3] = dbltocmplx(0.0, 0.0);
        Zero[4] = dbltocmplx(0.0, 0.0);
        Zero[5] = dbltocmplx(0.0, 1.05);
        Zero[6] = dbltocmplx(0.0, -1.05);
        Zero[7] = dbltocmplx(0.0, 0.0);
        Zero[8] = dbltocmplx(0.0, 0.0);

        npole = 17;
        Pole[1] = dbltocmplx(-0.13, 0.0);
        Pole[2] = dbltocmplx(-6.02, 0.0);
        Pole[3] = dbltocmplx(-8.66, 0.0);
        Pole[4] = dbltocmplx(-35.2, 0.0);
        Pole[5] = dbltocmplx(-100.0, 0.0);
        Pole[6] = dbltocmplx(-3.93, 0.0);
        Pole[7] = dbltocmplx(-0.282, 0.0);
        Pole[8] = dbltocmplx(-0.201, 0.241);
        Pole[9] = dbltocmplx(-0.201, -0.241);
        Pole[10] = dbltocmplx(-0.134, 0.1);
        Pole[11] = dbltocmplx(-0.134, -0.1);
        Pole[12] = dbltocmplx(-0.0251, 0.0);
        Pole[13] = dbltocmplx(-0.00942, 0.0);
        Pole[14] = dbltocmplx(-0.24, 0.58);
        Pole[15] = dbltocmplx(-0.58, 0.24);
        Pole[16] = dbltocmplx(-0.58, -0.24);
        Pole[17] = dbltocmplx(-0.24, -0.58);

    } else {
        nerr = 2105;
        setmsg("ERROR", nerr);
        apcmsg("SRO:", 5);
        apcmsg(subtyp, subtyp_s);
        goto L_8888;
    }

    /*   .....Compute transfer function.....
     * */
    getrand(nfreq, delfrq, const_, nzero, zero, npole, pole, xre, xim);

  L_8888:
    return;
}                               /* end of function */
