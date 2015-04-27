
#include <string.h>

#include "sam.h"
#include "fir.h"
#include "hdr.h"
#include "bool.h"
#include "amf.h"
#include "dfm.h"

#include "msg.h"
#include "dbh.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/
xfir(nerr)
     int *nerr;
{
    int lusrec;
    int isym, j, jb, jdfl, jf, ncfir4, nfft, nlenfl, nlpow2, npow2, nptsmx;
    float fac, firim, firrl, rnpow2, sigim, sigrl;
    sac *s;
    float *Sacmem1, *Sacmem2, *Sacmem3, *Sacmem4;
    float *scr, *srl;
        /*=====================================================================
	 * PURPOSE:  To execute the action command FIR.
	 *           This command applies an FIR filter to data in memory.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:    MCPFN
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SAM:     LRQREC, KNMFIR
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, LCLOG2, LCCHAR, CFMT, CRESP
	 *             RFIR, VFEVEN, GETFIL, GTOUTM, ALLAMB, WRTXTT, EDECIM
	 *             COPY, EXTRMA, PUTFIL, FILL, CPFT, RELAMB
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    910229:  Changed include file "dfir" back to "fir" (wct).
	 *    910228:  Changed include file "fir" to "dfir" (wct).
	 *    870923:  Deleted ".saf" from aux file names.
	 *    850801:  Changes due to new memory manager.
	 *    820621:  Changed to newest set of parsing and checking functions.
	 *    820331:  Combined "parse" and "control" modules.
	 *    810414:  Minor changes relating to new CMSAM.
	 *    810205:  Replaced call to ZFILNM with more general ZCHSTR.
	 *    801210:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  820331
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;
    scr = NULL;
    srl = NULL;
    /* PARSING PHASE: */

    /* - Loop on each token in command: */

  L_1000:
    if (lcmore(nerr)) {

        /* -- "REC/FFT":  selects recursive/transform method. */
        if (lclog2("R$", 3, "F$", 3, &cmsam.lrqrec)) {

            /* -- "filename":  set name of FIR filter coefficient file. */
        } else if (lcchar(cmsam.knmfir, sizeof(cmsam.knmfir))) {

            /* -- Bad syntax. */
        } else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();

        }
        goto L_1000;

    }

    /* - The above loop is over when one of two conditions has been met:
     *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
     *   (2) All the tokens in the command have been successfully parsed. */

    if (*nerr != 0)
        goto L_8888;

    /* CHECKING PHASE: */

    /* - Read disk file with FIR filter coefficients. */

    rfir(cmsam.knmfir, MCPFN + 1, MFIR, cmfir.cfir, &cmfir.ncfir, &cmfir.dtfir,
         kmfir.kidfir, 81, nerr);
    if (*nerr != 0)
        goto L_8888;
    ncfir4 = (cmfir.ncfir / 2) * 4 + 1;
    if (memcmp(kmfir.kidfir, "FIR-FILT", 8) == 0) {
        isym = 1;
    } else {
        isym = -1;
    }

    /* - Make sure all files:
     * -- are evenly spaced time series or x vs y files.
     * -- have the same sampling rate as the one used to generate
     *    the filter coefficients.
     *    Also compute the maximum number of data points. */

    vfeven(nerr);
    if (*nerr != 0)
        goto L_8888;

    vfmaxn(MLARGE, &nptsmx, nerr);
    if (*nerr != 0)
        goto L_8888;

        /**del**      DO 2000 JDFL=1,NDFL
	 **del**        CALL GETFIL(JDFL,.FALSE.,NLEN,NDX1,NDX2,NERR)
	 **del**        IF(NERR.NE.0)GO TO 8888
	 **del**        IF(DELTA.NE.DTFIR)THEN
	 **del**          NERR=1601
	 **del**          CALL SETMSG('ERROR',NERR)
	 **del**          CALL LNUMCL(KDFL,JDFL,IC1,IC2)
	 **del**          CALL APCMSG(KDFL(IC1:IC2))
	 **del**          GO TO 8888
	 **del**        ENDIF
	 **del** 2000   CONTINUE */

    /* EXECUTION PHASE: */

    /* - FIR filters can be applied recursively or by using FFT's.
     *   The recursive method is used when:
     * -- the user requests it.
     * -- there is no room in SACMEM array for the scratch arrays
     *    needed for FFT.
     * -- the size of the data arrays are too large for FFT. */

    /* - Check to see if FFT can be used (if requested). */

    if (cmsam.lrqrec) {
        lusrec = TRUE;
    } else {
        nfft = next2(nptsmx + ncfir4);
        if (nfft <= MFFT) {
            scr = (float *) malloc(sizeof(float) * 4 * nfft);
            if (*nerr == 0) {
                lusrec = FALSE;
                srl = scr;
            } else {
                lusrec = TRUE;
            }
        } else {
            lusrec = TRUE;
        }
    }

    /* - Check to see if recursive method can be used (if requested). */

    if (lusrec) {
        scr = (float *) malloc(sizeof(float) * nptsmx);
        if (!cmsam.lrqrec) {
            setmsg("WARNING", 1602);
            outmsg();
        }
    }

    /* - Main loop using recursive method: */

    if (lusrec) {

        for (jdfl = 1; jdfl <= saclen(); jdfl++) {

            /* -- Get next file in DFL, moving header to CMHDR. */
            if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
                goto L_7777;
            }
            //getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

            /* -- Apply FIR filter recursively to this data file, storing the
             *    result into the scratch space. */

            edecim(s->y, s->h->npts, 1, scr, &nlenfl, cmfir.cfir, cmfir.ncfir,
                   isym);

            /* -- Move the filtered data into location of the original data. */

            copy_float(scr, s->y, s->h->npts);

            /* -- Adjust header of file in DFL. */

            extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax,
                   &s->h->depmen);

        }

    } else {

        /* - Main loop using FFT method. */

        nlpow2 = 0;
        for (jdfl = 1; jdfl <= saclen(); jdfl++) {

            if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
                goto L_7777;
            }
            //getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

            /* -- Compute power of two greater than number of points. */
            npow2 = next2(s->h->npts + ncfir4);

            /* -- Do FFT on FIR coefficients if length of transform has changed. */

            if (npow2 != nlpow2) {
                fill(&scr[2 * nfft], npow2, 0.);
                fill(&scr[3 * nfft], npow2, 0.);
                fac = isym;
                scr[2 * nfft] = Cfir[1];
                jf = 1;
                jb = npow2 - 1;
                for (j = 2; j <= cmfir.ncfir; j++) {
                    scr[2 * nfft + jf] = Cfir[j];
                    //*(Sacmem+jf) = Cfir[j];
                    //*(Sacmem+jb) = fac*Cfir[j];
                    scr[2 * nfft + jb] = fac * Cfir[j];
                    jf = jf + 1;
                    jb = jb - 1;
                }
                cpft(&scr[2 * nfft], &scr[3 * nfft], npow2, 1, cmsam.ifwd);
            }

            /* -- Transform the signal. */

            copy_float(s->y, srl, s->h->npts);
            fill(&srl[s->h->npts], npow2 - s->h->npts, 0.);
            fill(&scr[nfft], npow2, 0.);
            cpft(srl, &scr[nfft], npow2, 1, cmsam.ifwd);

            /* -- Multiply transform of signal and filter; store in signal arrays. */

            Sacmem1 = scr;
            Sacmem2 = scr + nfft;
            Sacmem3 = scr + 2 * nfft;
            Sacmem4 = scr + 3 * nfft;
            for (j = 0; j <= (npow2 - 1); j++) {
                sigrl = *(Sacmem1 + j);
                sigim = *(Sacmem2 + j);
                firrl = *(Sacmem3 + j);
                firim = *(Sacmem4 + j);
                *(Sacmem1 + j) = sigrl * firrl - sigim * firim;
                *(Sacmem2 + j) = sigrl * firim + sigim * firrl;
            }

            /* -- Perform inverse transform on filtered signal. */

            cpft(Sacmem1, Sacmem2, npow2, 1, cmsam.ibwd);

            /* -- Store normalized results back into data array. */

            rnpow2 = 1. / (float) (npow2);
            Sacmem1 = s->y;
            Sacmem2 = srl;
            for (j = 0; j <= (s->h->npts - 1); j++) {
                *(Sacmem1++) = rnpow2 * (*(Sacmem2++));
            }

            /* -- Adjust header of file in DFL. */

            extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax,
                   &s->h->depmen);

            nlpow2 = npow2;
        }

    }

  L_7777:
    FREE(scr);

    /* - Calculate and set new range of dependent variable. */

    setrng();

  L_8888:
    return;

}                               /* end of function */
