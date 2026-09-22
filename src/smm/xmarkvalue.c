
#include <stdio.h>
#include <string.h>

#include "smm.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "msg.h"
#include "clf.h"
#include "bot.h"
#include "cpf.h"
#include "dff.h"


SMM_EXTERN
DFM_EXTERN

void /*FUNCTION*/
xmarkvalue(int *nerr)
{
    char kdescr[9];
    int ifpick, ikpick, index, ipick, j, jdfl, nlnatw, nofatw;
    double tmax, tmin;
    char *tmp;
    sac *s;
        /*=====================================================================
	 * PURPOSE: To parse and execute the action command MARKVALUE.
	 *          This command marks the first occurance of a data value.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL, KDFL
	 *    SMM:     LMTW, KMTW, OMTW
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    HDR:     
	 *    SMM:     KVMARK, LGEDATA
	 *    MEM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    870128:  Added use of measurement time window (MTW).
	 *    861128:  Moved to SMM as an internal SAC command called MARKVALUE.
	 *    841207:  Reworked into new XSC format.
	 *    830124:  Original version called LDV.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    /* PARSING PHASE: */

    /* - Loop on each token in command: */

  L_1000:
    if (lcmore(nerr)) {

        /* -- "GE v":  search for first point that is greater than or equal to v. */
        if (lkreal("GE$", 4, &cmsmm.value)) {
            cmsmm.lgedata = TRUE;

            /* -- "LE v":  search for first point that is less than or equal to v. */
        } else if (lkreal("LE$", 4, &cmsmm.value)) {
            cmsmm.lgedata = FALSE;

            /* -- "TO hdrvar":  the name of the header variable to store time. */
        } else
            if (lklist("TO$", 4, (char *) kmdfm.kpick[7], 9, MPICK - 7, &ipick))
        {
            strcpy(kmsmm.kvmark, kmdfm.kpick[ipick + 6]);

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

    /* - Test for a non-null data file list. */

    vflist(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Make sure each file is a time series file. */

    vftime(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* EXECUTION PHASE: */

    /* - Determine offset in header arrays for requested time pick. */

    ipick = nequal(kmsmm.kvmark, (char *) kmdfm.kpick, 9, MPICK);
    ifpick = ipick + 10 - 7;
    ikpick = ipick + 6 - 7;

    /* - Format description for storage in header. */

    if (cmsmm.lgedata) {
        snprintf(kdescr, 9,"GE %5.1f", cmsmm.value);
    } else {
        snprintf(kdescr, 9,"LE %5.1f", cmsmm.value);
    }

    /* - Perform the requested function on each file in DFL. */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {

        /* -- Get next file from the memory manager.
         *    (Header is moved into common blocks CMHDR and KMHDR.) */
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        //getfil( jdfl, TRUE, &nlen, &ndxy, &ndxx, nerr );

        /* -- Determine measurement window. */
        if (cmsmm.lmtw) {
            getatw((char *) kmsmm.kmtw, 9, cmsmm.omtw, &tmin, &tmax, &nofatw,
                   &nlnatw, nerr);
            if (*nerr != 0)
                goto L_8888;
        } else {
            nofatw = 0;
            nlnatw = s->h->npts;
        }

        /* -- Perform search of data file. */
        index = -1;
        if (cmsmm.lgedata) {
            for (j = nofatw; j <= (nofatw + nlnatw - 1); j++) {
                if (s->y[j] >= cmsmm.value) {
                    index = j;
                    goto L_4000;
                }
            }
        } else {
            for (j = nofatw; j <= (nofatw + nlnatw - 1); j++) {
                if (s->y[j] <= cmsmm.value) {
                    index = j;
                    goto L_4000;
                }
            }
        }

        /* -- Store in the requested header field. */
      L_4000:
        if (index >= 0) {
            sac_set_float(s, ifpick, B(s) + (double) (index) * DT(s));
            strcpy(khdr(s, ikpick), kdescr);
        } else {
            sac_set_float(s, ifpick, SAC_FLOAT_UNDEFINED);
            strcpy(khdr(s, ikpick), SAC_CHAR_UNDEFINED);
            setmsg("WARNING", 1);
            apcmsg("Could not find value", 21);
            apfmsg(cmsmm.value);
            apcmsg("in file", 8);
            tmp = s->m->filename;
            apcmsg2(tmp, strlen(tmp) + 1);
            wrtmsg(stdout);
        }

    }

  L_8888:
    return;

}                               /* end of function */
