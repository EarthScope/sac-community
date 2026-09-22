
#include <string.h>

#include "sam.h"
#include "dfir.h"
#include "amf.h"
#include "bool.h"

#include "dbh.h"
#include "cpf.h"

#define	IPULSE	100

SAM_EXTERN
DFIR_EXTERN

void /*FUNCTION*/
fdlp(float *memptr[], int mxmptr, float *userData, int *nerr)
{
    int idx, jdx;
    float dummy, highSamplingFreq, lowSamplingFreq;
    char s1[3];
    double tmp;

        /*=====================================================================
	 * PURPOSE:  Creates filter response files in memory by evaluating
	 *           filter command line parameters for lowpass.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    SAM:     KTPIIR, MTPIIR, KPROTYP, MPROTYP, NDATPTS, NIMPPTS 
	 *             FDDELTA
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SAM:     CFLP, ITPLP, NPOLLP, NPASLP, TBWLP, ATNLP
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  ALLAMB, INSPECT, DESIGN, ZERO, APPLY
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *  mxmptr:  Number of entries in memptr. [i]
	 *  dummy:   Dummy array for passing to inspect with digital data. [r]
	 *  sampling:The delta. [r]
	 *  ipulse:  The array location (as a parameter) of the impulse response 
	 *           onset. [100]
	 *=====================================================================
	 * GLOBAL VARIABLES:
	 *  dfir:  
	 *         nsects:  Number of second order sections in impulse. [i]
	 *         sn:       Numerator coefficients. [r]
	 *         sd:       Denominator coefficients. [r]
	 *         filttype: Filter type. [character*2]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *  901219 Added included file dfir for saving coefficients, nsects, and
	 *         filter type.
	 *  901116 Added delta option for selecting sampling interval.
	 *  901001 Original version.
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;
    strcpy(cmfir3.filttype, "LP");
    strcpy(s1, "  ");

    /* - Loop on each token in command: */

    while (lcmore(nerr)) {

        /* -- CORNER v1 v2:  define new corner frequency. */
        if (lkrrc("CORNER$", 8, 0., VLARGE, &cmsam.cflp)) {     /* do nothing */
        }

        /* -- BU/BE/C1/C2:  change type of IIR filter to perform. */
        else if (lclist((char *) kmsam.ktpiir, 9, MTPIIR, &cmsam.itplp)) {      /* do nothing */
        }

        /* -- NPOLES n:  define npoles of poles in filter. */
        else if (lkirc("NPOLES$", 8, 1, 10, &cmsam.npollp)) {   /* do nothing */
        }

        /* -- TRANBW v:  define new transition bandwidth. */
        else if (lkrrc("TRANBW$", 8, 0., VLARGE, &cmsam.tbwlp)) {       /* do nothing */
        }

        /* -- ATTEN v:  define new filter attenuation factor. */
        else if (lkrrc("ATTEN$", 7, 1., VLARGE, &cmsam.atnlp)) {
        }

        /* -- PASSES n:  Set number of filter passes. */
        else if (lkirc("PASSES$", 8, 1, 2, &cmsam.npaslp)) {    /* do nothing */
        }

        /* -- DELTA v:  Set the delta. */
        else if (lkrrc("D#ELTA$", 8, VSMALL, VLARGE, &tmp)) {
            cmsam.fddelta = (float) tmp;
        }

        /* -- Bad syntax. */
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }

    /* - The above loop is over when one of two conditions has been met:
     *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
     *   (2) All the tokens in the command have been successfully parsed. */

    if (*nerr != 0)
        goto L_8888;

    /* set userData */
    userData[0] = 1;
    userData[1] = cmsam.itplp;
    userData[2] = cmsam.npollp;
    userData[3] = cmsam.npaslp;
    userData[4] = cmsam.tbwlp;
    userData[5] = cmsam.atnlp;
    userData[6] = cmsam.fddelta;
    userData[7] = cmsam.cflp;
    userData[8] = -12345.0;

    /* - Compute each filter response prototype and put results in memory. */

    /* -- Allocate and load x and y data arrays for analog signals.
     *    This will be memptr(1 thru 6) */

    jdx = 1;
    for (idx = 0; idx < 3; idx++) {
        memptr[jdx - 1] = (float *) malloc(sizeof(float) * NDATPTS);
        memptr[jdx + 1 - 1] = (float *) malloc(sizeof(float) * NDATPTS);
        strncpy(s1, kmsam.ktpiir[cmsam.itplp - 1], 2);

        lowSamplingFreq = 0.0;
        highSamplingFreq = 1. / (2. * cmsam.fddelta);
        inspect(cmsam.npollp, "LP", s1, cmsam.atnlp, cmsam.tbwlp, 0.0,
                cmsam.cflp, cmsam.fddelta, (char *) kmsam.kprotyp[idx], NDATPTS,
                &lowSamplingFreq, &highSamplingFreq, "LINEAR", memptr[jdx - 1],
                memptr[jdx + 1 - 1]);
        jdx = jdx + 2;
    }

    /* -- Allocate and load y data array only for digital signals.
     *    This will be memptr(7 thru 9) */

    for (idx = 3; idx < MPROTYP; idx++) {
        memptr[idx + 4 - 1] = (float *) malloc(sizeof(float) * NDATPTS);
        strncpy(s1, kmsam.ktpiir[cmsam.itplp - 1], 2);

        lowSamplingFreq = 0.0;
        highSamplingFreq = 1. / (2. * cmsam.fddelta);
        inspect(cmsam.npollp, "LP", s1, cmsam.atnlp, cmsam.tbwlp, 0.0,
                cmsam.cflp, cmsam.fddelta, (char *) kmsam.kprotyp[idx], NDATPTS,
                &lowSamplingFreq, &highSamplingFreq, "LINEAR",
                memptr[idx + 4 - 1], (float *) &dummy);
    }

    /* - Correct for the last datum in the group delay analog and digital
     *   traces. We tried to use L'Hopitals rule in afr, called in inspect,
     *   but it didn't work. Set the last data value to equal the second. */

    memptr[5 - 1][NDATPTS - 1] = memptr[5 - 1][NDATPTS - 2];
    memptr[9 - 1][NDATPTS - 1] = memptr[9 - 1][NDATPTS - 2];

    /* - Correct for the last data value of the digital phase response. */
    memptr[8 - 1][NDATPTS - 1] = memptr[8 - 1][NDATPTS - 2];

    /* - Create the impulse response */
    memptr[mxmptr - 1] = (float *) malloc(sizeof(float) * NIMPPTS);
    strncpy(s1, kmsam.ktpiir[cmsam.itplp - 1], 2);

    design(cmsam.npollp, "LP", s1, cmsam.atnlp, cmsam.tbwlp, 0.0, cmsam.cflp,
           cmsam.fddelta, cmfir3.sn, cmfir3.sd, &cmfir3.nsects);
    zero(memptr[mxmptr - 1], NIMPPTS);
    memptr[mxmptr - 1][IPULSE - 1] = 1.0;
    apply(memptr[mxmptr - 1], NIMPPTS, FALSE, cmfir3.sn, cmfir3.sd,
          cmfir3.nsects);

  L_8888:
    return;

}                               /* end of function */
