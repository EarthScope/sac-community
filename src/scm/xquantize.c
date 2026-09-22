
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scm.h"
#include "dfm.h"
#include "hdr.h"
#include "amf.h"
#include "bool.h"

#include "co.h"
#include "msg.h"
#include "clf.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"

SCM_EXTERN

void /*FUNCTION*/
xquantize(int *nerr)
{
    int irange, ivalue, j, jdfl, jqgain, nclip, nqgain;
    float factor, half, temp;

    char *tmp;
    sac *s;

        /*=====================================================================
	 * PURPOSE: To parse and execute the action command QUANTIZE.
	 *          This command quantizes the data in DFL.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    HDR:     DEPMIN, DEPMAX, DEPMEN
	 *    MEM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    870217:  Modified error and warning messages.
	 *    861129:  Converted to internal SAC command.
	 *    860417:  Modified to fit into new XSC format.
	 *    830124:  Modified to include new set of parsing/checking functions.
	 *    810624:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870217
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;
    nqgain = 0;
    /* PARSING PHASE */

    /* - Loop on each token in command. */

  L_1000:
    if (lcmore(nerr)) {

        /* -- "GAINS ilist":  list of allowed gains. */
        if (lkia("GAINS$", 7, 1, MQGAIN, cmscm.iqgain, &nqgain)) {
            cmscm.iqgain[nqgain] = 1;

            /* -- "LEVEL v":  quantization level for lowest gain. */
        } else if (lkreal("LEVEL$", 7, &cmscm.qlevel)) {

            /* -- "MANTISSA n":  number of bits in mantissa. */
        } else if (lkint("MANTISS#A$", 9, &cmscm.nqmant)) {

            /* -- Bad syntax. */
        } else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();

        }
        goto L_1000;

    }

    /* - The above loop is over when one of two conditions has beenmet:
     *   (1) An error in parsing has occurred.  In this case NERR is > 0
     *   (2) All the tokens in the command have been successfully parsed */

    if (*nerr != 0)
        goto L_8888;

    /* CHECKING PHASE: */

    /* - Test for a non-null data file list. */

    vflist(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Make sure each file is an evenly spaced time series file. */

    vfeven(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Make sure gains are monotonically decreasing. */

    for (j = 1; j < nqgain; j++) {
        if (cmscm.iqgain[j] >= cmscm.iqgain[j - 1]) {
            *nerr = 2006;
            setmsg("ERROR", *nerr);
            goto L_8888;
        }
    }

    /* EXECUTION PHASE: */

    /* - Convert number of bits in mantissa to maximum integer range. */

    irange = (ipow(2, cmscm.nqmant) - 1) / 2;

    /* - Calculate scale factors. */

    factor = cmscm.qlevel * (float) (cmscm.iqgain[0]);

    /* - For each file in DFL: */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {

        /* -- Get next file from the memory manager.
         *   (Header is moved into common blocks CMHDR and KMHDR.) */
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        //getfil( jdfl, TRUE, &nlen, &ndxy, &ndxx, nerr );

        /* -- For each data point in file:
         *    (1) Determine proper gain.
         *    (2) Store quantized value (using truncation model.) */

        nclip = 0;
        for (j = 0; j < s->h->npts; j++) {
            jqgain = 1;
            temp = s->y[j];
            half = sign(0.5, temp);
          L_3000:
            ivalue = (int) (half + temp * (float) (cmscm.iqgain[jqgain-1]) / factor);
            if (labs(ivalue) > irange) {
                jqgain = jqgain + 1;
                if (jqgain <= nqgain)
                    goto L_3000;
                nclip = nclip + 1;
                ivalue = irange + 1;
            }
            s->y[j] = (float) (ivalue) * factor / (float) (cmscm.iqgain[jqgain-1]);
        }

        /* -- Write warning message if any data points clipped. */

        if (nclip > 0) {
            setmsg("WARNING", 1);
            apimsg(nclip);
            apcmsg("data point(s) clipped in file:", 31);
            tmp = s->m->filename;
            apcmsg2(tmp, strlen(tmp) + 1);
            wrtmsg(stdout);
        }

        /* -- Update any header fields that may have changed. */

        extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax,
               &s->h->depmen);

    }

    /* - Calculate and set new range of dependent variable. */

    setrng();

  L_8888:
    return;

}                               /* end of function */
