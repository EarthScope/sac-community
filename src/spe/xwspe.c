
#include "spe.h"
#include "hdr.h"
#include "amf.h"

#include "msg.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"

SPE_EXTERN

extern sac *spe;
extern float *spespe;
void /*FUNCTION*/
xwspe(nerr)
     int *nerr;
{
    char *filename;
    sac *s;
        /*=====================================================================
	 * PURPOSE:  To execute the action command WRITESPE.
	 *           This command writes the spectral estimate to disk.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag.
	 *=====================================================================
	 * MODULE/LEVEL:  spe/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    MCPFN
	 *    hdr:     npts, b, delta, iftype
	 *    spe:     knmspe, lspe, samfrq, nlnspe, ndxspe
	 *    mem:     sacmem
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    hdr:     nsnpts, sb, sdelta, iftype, npts, delta, b, e
	 *    spe:     knmspe
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, cfmt, cresp, lcchar, setmsg, wsac0
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    060627:  Produce error message explicitly after wsac0 failure (grh/ub)
         *    880422:  Writing full header instead of just the minimum. 
	 *    820817:  Changed to newest set of parsing and checking functions.
	 *    810120:  Changed to output message retrieval from disk.
	 *    801219:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  880422
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    /* PARSING PHASE: */

    /* - Loop on each token in command: */

    while (lcmore(nerr)) {

        /* -- "filename":  define new filename for write. */
        if (lcchar(kmspe.knmspe, sizeof(kmspe.knmspe))) {       /* do nothing */
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

    /* CHECKING PHASE: */

    /* - Make sure a spectral estimate has been calculated. */

    if (!cmspe.lspe) {
        *nerr = 5004;
        setmsg("ERROR", *nerr);
        goto L_8888;
    }

    /* EXECUTION PHASE: */

    /* - Set up the header variables for the spectral estimate. */

    s = sac_new();
    sac_header_copy(s, spe);
    s->h->nsnpts = spe->h->npts;
    s->h->sb = spe->h->b;
    s->h->sdelta = spe->h->delta;

    s->h->iftype = ITIME;
    s->h->delta = cmspe.samfrq / (float) (cmspe.nlnspe - 1);
    s->h->npts = cmspe.nlnspe / 2 + 1;
    s->h->b = 0.;
    s->h->e = spe->h->b + spe->h->delta * (float) (spe->h->npts - 1);

    s->y = spespe;
    sac_extrema(s);

    /* - Write spectral estimate to disk. */
    filename = fstrdup(kmspe.knmspe, MCPFN + 1);
    sac_write_r(s, filename, SAC_WRITE_HEADER_AND_DATA, SAC_NO_BYTESWAP_FILE,
                nerr);

    if (*nerr != 0) {
        error(*nerr, "%s", filename);
    }
    s->y = NULL;
    sac_free(s);
    FREE(filename);

  L_8888:
    return;

}                               /* end of function */
