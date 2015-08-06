
#include <string.h>

#include "icm.h"
#include "bool.h"
#include "hdr.h"
#include "amf.h"
#include "dfm.h"
#include "SacHeader.h"

#include "EVRESPnames.h"

#include "bot.h"
#include "ucf.h"
#include "msg.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"

ICM_EXTERN

void DisconnectFromOracleTransfer(void);

void /*FUNCTION*/
xtransfer(nerr)
     int *nerr;
{
    int ldone;
    int iprewu, jdfl, maxn, nfft, nfreq, nra, ntused;

    double *sre, *sim, *xre, *xim;
    sac *s;
        /*=======================================================================
	 * PURPOSE: To parse and execute the action command transfer.
	 *          This command deconvolves a seismogram into ground displace-
	 *          ment, then convolves the displacementgram with the response
	 *          of another instrument.
	 *=======================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag.  Set to 0 if no error occurred. [i]
	 *=======================================================================
	 * MODULE/LEVEL:  icm/2
	 *=======================================================================
	 * GLOBAL INPUT:
	 *    mach:    mlarge
	 *    icm:     kinstr, ninstr
	 *    dfm:     ndfl
	 *=======================================================================
	 * GLOBAL OUTPUT:
	 *    icm:     fpfrom, lfpfrom, ipfrom, lipfrom, kpfrom, lkpfrom,
	 *             fpto, lfpto, ipto, lipto, kpto, lkpto,
	 *             freq, lfreql, lprew, iprew
	 *    hdr:     depmin, depmax, depmen
	 *    mem:     sacmem
	 *=======================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, cfmt, cresp
	 *=======================================================================
	 * MODIFICATION HISTORY:
	 *    871023:  Major restructuring of instrument parameter storage.
	 *    870316:  Minor modifications before putting into SAC.
	 *    861105:  Original version in TESTSHELL.
	 *=======================================================================
	 * DOCUMENTED/REVIEWED:  871023
	 *======================================================================= */
    /* PROCEDURE: */
    *nerr = 0;
    xim = NULL;
    xre = NULL;
    sim = NULL;
    sre = NULL;
    /* PARSING PHASE: */

    /* - Loop on each token in command: */

    ldone = FALSE;
    clearEVRESPstrucs();

    while (lcmore(nerr)) {

        /* -- "FROM ...":  set 'FROM' instrument information. */
        if (lckey("FROM#$", 7)) {
            setTransferDirection(FROM);
            getins((char *) kmicm.kinstr, 9, cmicm.ninstr, &ldone, cmicm.fpfrom,
                   cmicm.lfpfrom, cmicm.ipfrom, cmicm.lipfrom,
                   (char *) kmicm.kpfrom, MCPFN + 1, cmicm.lkpfrom, nerr);
            if (*nerr != 0)
                goto L_8888;
            if (ldone)
                break;
        }

        /* -- "TO ...":  set 'TO' instrument information. */
        else if (lckey("TO#$", 5)) {
            setTransferDirection(TO);
            getins((char *) kmicm.kinstr, 9, cmicm.ninstr, &ldone, cmicm.fpto,
                   cmicm.lfpto, cmicm.ipto, cmicm.lipto, (char *) kmicm.kpto,
                   MCPFN + 1, cmicm.lkpto, nerr);
            if (*nerr != 0)
                goto L_8888;
            if (ldone)
                break;
        }

        /* -- "FREQLIMITS v1 v2 v3 v4":  set frequency limits. */
        else if (lkra("FREQ#LIMITS$", 13, 4, 4, cmicm.freq, &nra)) {
            if (nra == 4)
                cmicm.lfreql = TRUE;
        }

        /* -- "PREWHITEN ON|OFF|n":  set prewhitener order. */
        else if (lklogi("PREW#HITEN$", 12, &cmicm.lprew, &cmicm.iprew)) {       /* do nothing */
        }

        /* -- Bad syntax. */
        else {
            cfmt("Illegal Option:", 17);
            cresp();
        }
    }

    /* - The above loop is over when one of two conditions has been met:
     *  (1) An error in parsing has occurred.  In this case nerr is > 0 .
     *  (2) All the tokens in the command have been successfully parsed. */

    if (*nerr != 0)
        goto L_8888;

    /* CHECKING PHASE: */

    /* - Test for completeness of input parameters */

    /* -- Assume 'NONE' as instrument type if one was omitted. */
    if (!cmicm.lkpfrom[0]) {
        fstrncpy(kmicm.kpfrom[0], MCPFN, "NONE", 4);
        cmicm.lkpfrom[0] = TRUE;
    }
    if (!cmicm.lkpto[0]) {
        fstrncpy(kmicm.kpto[0], MCPFN, "NONE", 4);
        cmicm.lkpto[0] = TRUE;
    }

    /* -- Check the "from" and "to" instrument parameters. */
    ckinst(cmicm.fpfrom, cmicm.lfpfrom, cmicm.ipfrom, cmicm.lipfrom,
           (char *) kmicm.kpfrom, MCPFN + 1, cmicm.lkpfrom, nerr);
    if (*nerr != 0)
        goto L_8888;
    ckinst(cmicm.fpto, cmicm.lfpto, cmicm.ipto, cmicm.lipto,
           (char *) kmicm.kpto, MCPFN + 1, cmicm.lkpto, nerr);
    if (*nerr != 0)
        goto L_8888;

    /* -- Check the frequency limits. */
    if ((cmicm.freq[0] >= cmicm.freq[1]) || (cmicm.freq[1] >= cmicm.freq[2]) ||
        (cmicm.freq[2] >= cmicm.freq[3])) {
        setmsg("WARNING", 2111);
        outmsg();
        clrmsg();
        cmicm.freq[0] = -2.;
        cmicm.freq[1] = -1.;
        cmicm.freq[2] = 1.e5;
        cmicm.freq[3] = 1.e6;
    }

    /* - Test for a non-null data file list. */

    vflist(nerr);

    /* - Make sure each file is an evenly spaced time series file. */

    vfeven(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - EXECUTION PHASE: */

    /* - Set up prewhitening order. */

    if (cmicm.lprew) {
        iprewu = cmicm.iprew;
    } else {
        iprewu = 0;
    }

    /* Determine the maximum number of data points in the files in DFL. */

    vfmax(&maxn, &ntused);

    /* - Set up scratch space for TRANSFER subroutine.  
     *   It needs four DOUBLE PRECISION arrays,two for the ffts that must be
     *   sized to the next power of two above the maximum number of points(nfft)
     *   and two that are half that size plus 1(nfreq.) */

    nfft = next2(maxn);
    nfreq = nfft / 2 + 1;
    sre = (double *) malloc(sizeof(double) * nfft);
    sim = (double *) malloc(sizeof(double) * nfft);
    xre = (double *) malloc(sizeof(double) * nfft);
    xim = (double *) malloc(sizeof(double) * nfft);
    if (!sre || !sim || !xre || !xim) {
        goto L_8888;
    }

    /* - Perform the requested function on each file in DFL. */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        //getfil( jdfl, TRUE, &nlen, &ndxy, &ndxx, nerr );

        /* -- Call the specific subroutine to work on this file.
         *    (This is usually a call to a subroutine.) */

        transfer(s->y, s->h->npts, s->h->delta, cmicm.fpfrom, cmicm.ipfrom,
                 kmicm.kpfrom, MCPFN + 1, cmicm.fpto, cmicm.ipto, kmicm.kpto,
                 MCPFN + 1, cmicm.freq, &iprewu, sre, sim, nfft, xre, xim,
                 nfreq, nerr);

        if (*nerr != 0)
            goto L_8888;

        /* -- Update any header fields that may have changed. */
        extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax,
               &s->h->depmen);

        if (kmicm.kpto[0][0]) {
            modcase(TRUE, kmicm.kpto[0], strlen(kmicm.kpto[0]), kmicm.kpto[0]);
            if (!strncmp(kmicm.kpto[0], "DIS", 3) ||
                !strncmp(kmicm.kpto[0], "NONE", 4))
                s->h->idep = IDISP;
            else if (!strncmp(kmicm.kpto[0], "VEL", 3))
                s->h->idep = IVEL;
            else if (!strncmp(kmicm.kpto[0], "ACC", 3))
                s->h->idep = IACC;
        }

        if (*nerr != 0)
            goto L_8888;

    }

    /* - Calculate and set new range of dependent variable. */

    setrng();

  L_8888:
    /* - Release scratch space. */
    FREE(sre);
    FREE(sim);
    FREE(xre);
    FREE(xim);

    DisconnectFromOracleTransfer();
    return;

}                               /* end of function */
