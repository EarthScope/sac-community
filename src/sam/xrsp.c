
#include <string.h>
#include <math.h>

#include "sam.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "ucf.h"
#include "msg.h"
#include "ssi.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"
#include <fstr.h>
#include "defs.h"
#include "string_utils.h"

DFM_EXTERN
SAM_EXTERN

void /*FUNCTION*/
xrsp(int *nerr)
{
    char krspnm[MCPFN + 1];
    int irsptp, nfreq;
    int n;
    char s1[4];
    char *tmp;
    string_list *list;
    static string_list *last_list = NULL;
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
        strcpy(kmsam.krsps1, ".am");
        strcpy(kmsam.krsps2, ".ph");
        irsptp = IAMPH;
    } else {
        strcpy(kmsam.krsps1, ".rl");
        strcpy(kmsam.krsps2, ".im");
        irsptp = IRLIM;
    }

    /* -- Clear data sets */
    sacclear();

    n = string_list_length(list);
    sac *so[2] = { NULL, NULL };

    for(int i = 0; i < n; i++) {
        int m = (cmsam.lrspe == TRUE) ? 1 : 2;
        so[0] = so[1] = NULL;
        tmp = string_list_get(list, i);
        for(int j = 0; j < m; j++) {
            if(!cmsam.lrspe) {
                snprintf(krspnm, sizeof(krspnm), "%s%s", tmp,
                         (j==0) ? kmsam.krsps1 : kmsam.krsps2);
            } else {
                strlcpy(krspnm, tmp, sizeof(krspnm));
            }
            so[j] = sac_read(krspnm, nerr);
            if(*nerr) {
                error(*nerr, "%s", krspnm);
                goto L_8888;
            }
        }
        sac *s = sac_new();
        sac_header_copy(s, so[0]);
        sac_meta_copy(s, so[0]);
        FREE(s->m->filename);
        s->m->filename = strdup(tmp);

        nfreq = s->h->npts;
        s->h->npts = 2 * (nfreq - 1);
        sac_set_float(s, SAC_B, 0.0);
        sac_set_float(s, SAC_E, DT(s) * (double) (nfreq - 1));
        s->h->iftype = irsptp;

        sac_alloc(s);

        /* -- Fill second half of first data component.
         *    This is either the real or the amplitude component
         *    and is therefore symmetric about its midpoint. */
        memcpy(s->y, so[0]->y, nfreq * SAC_DATA_SIZE);
        for(int j = 0; j < nfreq-2; j++) {
            s->y[s->h->npts-1-j] =  s->y[1+j];
        }

        if(cmsam.lrspe) {
            /* -- If this is a SPE file:
             *    (1) Take the square root of each data point. This converts
             *        it from a power estimate to an amplitude estimate.
             *    (2) Recompute the extrema stored in the header.
             *    (3) Zero out the second (phase) data component.
             *    (4) Loop to the next file in the list. */
            for (int j = 0; j < s->h->npts; j++) {
                s->y[j] = sqrt(s->y[j]);
            }
            memset(s->x, 0, s->h->npts * SAC_DATA_SIZE);
        } else {
            memcpy(s->x, so[1]->y, nfreq * SAC_DATA_SIZE);
            for(int j = 0; j < nfreq - 2 ; j++) {
                s->x[s->h->npts-1-j] = -s->x[1+j];
            }
        }

        sac_free(so[0]);
        sac_free(so[1]);

        sac_extrema(s);
        sacput(s);
    }

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
