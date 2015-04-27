
#include <string.h>
#include <math.h>

#include "sam.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "ucf.h"
#include "ssi.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"

void /*FUNCTION*/
xrsp(nerr)
     int *nerr;
{
    char krspnm[MCPFN + 1];
    int irsptp, jdx, jdfl, junk, nfreq, nlcdsk, nrspnm, nun;
    int n;
    char s1[4];
    char *tmp;
    float *Sacmem1, *Sacmem2;
    string_list *list;
    static string_list *last_list = NULL;
    sac *s;
    if (!last_list) {
        last_list = string_list_init();
    }

    list = NULL;

        /*=====================================================================
	 * PURPOSE:  To execute the action command READSP.
	 *           This command reads separate spectral component files.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag
	 *=====================================================================
	 * MODULE/LEVEL: SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    HDR:     IAMPH, IRLIM
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    DFM:     NDFL, KDFL, NCOMP, NDXHDR, NLNDTA, NDXDTA
	 *    HDR:     All.
	 *    MEM:     SACMEM
	 *    SAM:     LRSPE, LRAMPH
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, LCLOG2, LCDFL, CFMT, CRESP
	 *             VFLIST, CLEARDFL, ZGTFUN, INDEXB, ZOPEN, RDHDR, ZRABS,
	 *             ALLAMB, GETFIL, PUTFIL
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    KRSPNM:  Name of disk file being read.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    921105:  Modification of 920420 din't work. Removed. The filenames
	 *             are modified.  so can't do a lfilesok before this.
	 *    920501:  Added memory delete on READERR NOFILES MEMORY DELETE cond.
	 *    920420:  Added test of file existence (lfilesok) before updating 
	 *             data-set info. Done in andicipation that a MORE option
	 *             may someday be implemented.
	 *    920316:  Added data-set storage logic.
	 *    880425:  Added option to read SPE files and convert to AMPH format.
	 *    871124:  Fixed bug that was not updating header correctly.
	 *    850614:  Major rewrite due to addition of memory manager.
	 *    820621:  Changed to newest set of parsing functions.
	 *    820331:  Combined "parse" and "control" modules.
	 *    810508:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  871124
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    strcpy(s1, "   ");

    /* - PARSING PHASE: */

    /* - Loop on each token in command: */

    while (lcmore(nerr)) {
        /* -- "AMPH|RLIM|SPE":  choose format for spectral files. */
        if (lckey("SPE#$", 6)) {
            cmsam.lrspe = TRUE;
            cmsam.lramph = TRUE;
        } else if (lclog2("AMPH#$", 7, "RLIM#$", 7, &cmsam.lramph))
            cmsam.lrspe = FALSE;

        /* -- TRUST:  whether or not to trust matching evids while
           moving data from SAC buffers to CSS buffers. */
        else if (lklog("TRUST#$", 8, &cmdfm.ltrust)) {  /* do nothing */
        }

        /* -- List of data files to be read. */
        else if ((list = lcdfl())) {    /* do nothing */
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

    if (!list) {
        list = string_list_init();
        string_list_extend(list, last_list);
    }

    if (cmdfm.nreadflag == RDB)
        cmdfm.nreadflag = HIGH;

    /* - EXECUTION PHASE: */

    /* - Define suffixes. */

    if (cmsam.lramph) {
        strcpy(kmsam.krsps1, ".am     ");
        strcpy(kmsam.krsps2, ".ph     ");
        irsptp = IAMPH;
    } else {
        strcpy(kmsam.krsps1, ".rl     ");
        strcpy(kmsam.krsps2, ".im     ");
        irsptp = IRLIM;
    }

    /* -- Clear data sets */
    sacclear();

    n = string_list_length(list);

    /* - Read headers (from first file in each pair) into memory. */
    for (jdfl = 1; jdfl <= n; jdfl++) {

        /* -- Determine character length of input file name. */
        tmp = string_list_get(list, jdfl - 1);
        fstrncpy(krspnm, MCPFN, tmp, strlen(tmp) + 1);
        nrspnm = min(strlen(krspnm), MCPFN - 3);

        /* -- Prepare new name */
        if (!cmsam.lrspe) {
            strncpy(s1, kmsam.krsps1, 3);
            subscpy(krspnm, nrspnm, -1, MCPFN, s1);
        }

        /* -- Open file. */
        zopen_sac(&nun, krspnm, strlen(krspnm) + 1, "RODATA", 7, nerr);
        if (*nerr != 0)
            goto L_8888;

        s = sac_new();
        s->m->filename = fstrdup(krspnm, nrspnm);
        sacput(s);

        /* -- Read header. */
        s->m->swap = rdhdr(s, &nun, krspnm, nerr);
        if (*nerr != 0)
            goto L_8888;
        if (s->h->nevid == -12345 || s->h->norid == -12345)
            cmdfm.nreadflag = LOW;

        /* -- Adjust certain header fields. */
        nfreq = s->h->npts;
        s->h->npts = 2 * (nfreq - 1);
        s->h->b = 0.;
        s->h->e = s->h->delta * (float) (nfreq - 1);
        s->h->iftype = irsptp;
        sac_alloc(s);

        /* -- Close file. */
        zclose(&nun, nerr);
        if (*nerr != 0)
            goto L_8888;
    }

    /* - Read data sections from both files in pair. */

    for (jdfl = 1; jdfl <= n; jdfl++) {

        if (!(s = sacget(jdfl - 1, FALSE, nerr))) {
            goto L_8888;
        }
        /* -- Get header from memory manager. */
        //getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );

        /* -- Open first file. */
        tmp = string_list_get(list, jdfl - 1);
        fstrncpy(krspnm, MCPFN, tmp, strlen(tmp) + 1);
        nrspnm = min(strlen(krspnm), MCPFN - 3);
        if (!cmsam.lrspe) {
            strncpy(s1, kmsam.krsps1, 3);
            subscpy(krspnm, nrspnm, -1, MCPFN, s1);
        }
        zopen_sac(&nun, krspnm, MCPFN + 1, "RODATA", 7, nerr);
        if (*nerr != 0)
            goto L_8888;

        /* -- Read data. */
        nlcdsk = SAC_HEADER_WORDS_FILE;
        nfreq = s->h->npts / 2 + 1;
        zrabs((int *) &nun, (char *) s->y, nfreq, (int *) &nlcdsk,
              (int *) nerr);
        if (s->m->swap) {       /* byteswap if necessary. */

            for (jdx = 0; jdx < nfreq; jdx++) {
                byteswap((void *) &s->y[jdx], 4);
            }
        }
        if (*nerr != 0)
            goto L_7777;

        /* -- Close file. */
        zclose(&nun, nerr);
        if (*nerr != 0)
            goto L_7777;

        /* -- Fill second half of first data component.
         *    This is either the real or the amplitude component
         *    and is therefore symmetric about its midpoint. */
        Sacmem1 = &s->y[1];
        Sacmem2 = &s->y[s->h->npts - 1];
        for (jdx = 1; jdx <= (nfreq - 2); jdx++) {
            *(Sacmem2--) = *(Sacmem1++);
        }

        /* -- If this is a SPE file:
         *    (1) Take the square root of each data point. This converts
         *        it from a power estimate to an amplitude estimate.
         *    (2) Recompute the extrema stored in the header.
         *    (3) Zero out the second (phase) data component.
         *    (4) Loop to the next file in the list. */

        if (cmsam.lrspe) {
            for (jdx = 0; jdx <= (s->h->npts - 1); jdx++) {
                s->y[jdx] = sqrt(s->y[jdx]);
            }
            extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax,
                   &s->h->depmen);

            for (jdx = 0; jdx <= (s->h->npts - 1); jdx++) {
                s->x[jdx] = 0.0;
            }
            goto L_4800;
        }

        /* -- Open second file in pair. */
        strncpy(s1, kmsam.krsps2, 3);
        subscpy(krspnm, nrspnm, -1, MCPFN, s1);

        zopen_sac(&nun, krspnm, MCPFN + 1, "RODATA", 7, nerr);
        if (*nerr != 0)
            goto L_7777;

        /* -- Read data (do not read header from second file.) */
        zrabs((int *) &nun, (char *) s->x, nfreq, (int *) &nlcdsk,
              (int *) nerr);
        if (s->m->swap) {       /* byteswap if necessary. */
            for (jdx = 0; jdx < nfreq; jdx++) {
                byteswap((void *) &s->x[jdx], 4);
            }
        }
        if (*nerr != 0)
            goto L_7777;

        /* -- Close file. */
        zclose(&nun, nerr);
        if (*nerr != 0)
            goto L_8888;

        /* -- Fill second half of second component (assymetric this time.) */
        Sacmem1 = &s->x[1];
        Sacmem2 = &s->x[s->h->npts - 1];
        for (jdx = 1; jdx <= (nfreq - 2); jdx++) {
            *(Sacmem2--) = -*(Sacmem1++);
        }

      L_4800:
        continue;
    }

  L_7777:
    zclose(&nun, &junk);

  L_8888:
    string_list_clear(last_list);
    string_list_extend(last_list, list);
    list = NULL;

    if (*nerr == 0) {
        cmdfm.nfilesFirst = 0;
        cmdfm.lread = TRUE;
        sacToSeisMgr(TRUE, FALSE, TRUE, nerr);
        cmdfm.lread = FALSE;
    }

    return;
}                               /* end of function */
