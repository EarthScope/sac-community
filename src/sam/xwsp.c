
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sam.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "ucf.h"
#include "ssi.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"
#include "defs.h"
#include "bot.h"
#include "string_utils.h"

DFM_EXTERN
SAM_EXTERN

void /*FUNCTION*/
xwsp(int *nerr)
{
    char kwspnm[2 * MCPFN + 1];
    char prefix[MCPFN + 1];
    int lconv;
    int index, ispectype, jdfl, nfreq;

    char *tmp;

    string_list *list;
    sac *s;

    list = NULL;
        /*=====================================================================
	 * PURPOSE:  To execute the action command WRITESP.
	 *           This command writes spectral files in memory to disk
	 *           as two separate files.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL, KDFL
	 *    HDR:     IFTYPE, IRLIM, IAMPH, NPTS, DELTA, B, E,
	 *             DEPMIN, DEPMAX, DEPMEN
	 *    SAM:     NWSPFL, KWSPFL, LWAMPH, LWRLIM, KWSPTP,
	 *             KWSPS1, KWSPS2, LWSPC1, LWSPC2
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SAM:     KWSPS1, KWSPS2
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LCLIST, LCKEY, LCDFL,
	 *             VFLIST, VFSPEC,
	 *             GTOUTM, GETFIL, TOAMPH, TORLIM, INDEXB,
	 *             ZDEST, ZNFILE, ZWABS, ZCLOSE, ZPUTC
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    KWSPNM:  Name of disk file being written.
	 *    LCONV:   .TRUE. if spectral file needs to be temporarily
	 *             converted from one type to another.
	 *    TEMP:    Scratch space used to store character information
	 *             before writing it to disk.
	 *=====================================================================
	 * KNOWN ERRORS:
	 * - POWER option not implemented.
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;
    /* PARSING PHASE: */
    ispectype = -1;
    /* - Loop on each token in command: */

    while (lcmore(nerr)) {

        /* -- "ASIS/RLIM/AMPH/RL/IM/AM/PH/POWER":  select components to write. */
        if (lclist((char *) kmsam.ksptpl, 9, cmsam.nsptpl, &index)) {
            strcpy(kmsam.kwsptp, kmsam.ksptpl[index - 1]);
            /* Case branch:   ASIS,RLIM,AMPH,RL  ,IM  ,AM  ,PH  ,POWER */
            switch (index) {
                case 1:
                    cmsam.lwamph = FALSE;
                    cmsam.lwrlim = FALSE;
                    cmsam.lwspc1 = TRUE;
                    cmsam.lwspc2 = TRUE;
                    break;
                case 2:
                    cmsam.lwamph = FALSE;
                    cmsam.lwrlim = TRUE;
                    cmsam.lwspc1 = TRUE;
                    cmsam.lwspc2 = TRUE;
                    strcpy(kmsam.kwsps1, ".rl");
                    strcpy(kmsam.kwsps2, ".im");
                    break;
                case 3:
                    cmsam.lwamph = TRUE;
                    cmsam.lwrlim = FALSE;
                    cmsam.lwspc1 = TRUE;
                    cmsam.lwspc2 = TRUE;
                    strcpy(kmsam.kwsps1, ".am");
                    strcpy(kmsam.kwsps2, ".ph");
                    break;
                case 4:
                    cmsam.lwamph = FALSE;
                    cmsam.lwrlim = TRUE;
                    cmsam.lwspc1 = TRUE;
                    cmsam.lwspc2 = FALSE;
                    strcpy(kmsam.kwsps1, ".rl");
                    strcpy(kmsam.kwsps2, ".na");
                    break;
                case 5:
                    cmsam.lwamph = FALSE;
                    cmsam.lwrlim = TRUE;
                    cmsam.lwspc1 = FALSE;
                    cmsam.lwspc2 = TRUE;
                    strcpy(kmsam.kwsps1, ".na");
                    strcpy(kmsam.kwsps2, ".im");
                    break;
                case 6:
                    cmsam.lwamph = TRUE;
                    cmsam.lwrlim = FALSE;
                    cmsam.lwspc1 = TRUE;
                    cmsam.lwspc2 = FALSE;
                    strcpy(kmsam.kwsps1, ".am");
                    strcpy(kmsam.kwsps2, ".na");
                    break;
                case 7:
                    cmsam.lwamph = TRUE;
                    cmsam.lwrlim = FALSE;
                    cmsam.lwspc1 = FALSE;
                    cmsam.lwspc2 = TRUE;
                    strcpy(kmsam.kwsps1, ".na");
                    strcpy(kmsam.kwsps2, ".ph");
                    break;
                case 8:
                    *nerr = 1012;
                    setmsg("ERROR", *nerr);
                    apcmsg(kmsam.kwsptp, 9);
                    cmsam.lwamph = TRUE;
                    cmsam.lwrlim = FALSE;
                    cmsam.lwspc1 = TRUE;
                    cmsam.lwspc2 = FALSE;
                    strcpy(kmsam.kwsps1, ".po");
                    strcpy(kmsam.kwsps2, ".na");
            }                   /* end switch */
        }

        /* end if ( lclist ) */
        /* -- "OVER":  overwrite data file list. */
        else if (lckey("OVER$", 6))
            cmsam.lwspov = TRUE;

        /* -- "COMMIT|RECALLTRACE|ROLLBACK":
           how to treat existing data */
        else if (lckeyExact("COMMIT", 7))
            cmdfm.icomORroll = COMMIT;
        else if (lckeyExact("RECALLTRACE", 12))
            cmdfm.icomORroll = RECALL;
        else if (lckeyExact("RECALL", 7))
            cmdfm.icomORroll = RECALL;
        else if (lckeyExact("ROLLBACK", 9))
            cmdfm.icomORroll = ROLLBACK;

        /* -- Parse list of file names for write. */
        else if ((list = lcdfl()))
            cmsam.lwspov = FALSE;

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

    /* - Check for null data file list. */

    vflist(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Check to make sure all files are spectral files. */

    vfspec(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Check length of write file list vs data file list. */

    if (!cmsam.lwspov) {
        if (string_list_length(list) != saclen()) {
            *nerr = 1312;
            setmsg("ERROR", *nerr);
            apimsg(cmsam.nwspfl);
            apimsg(saclen());
            goto L_8888;
        }
    }

    /* EXECUTION PHASE: */

    /* - Commit or rollback data according to lmore and cmdfm.icomORroll */
    alignFiles(nerr);
    if (*nerr)
        return;

    /* - Perform the requested function on each file in DFL. */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {

        /* -- Get the next file in DFL, moving header to CMHDR. */
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            return;
        }
        //getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

        /* -- Convert spectral file type if needed. */

        if (cmsam.lwamph && s->h->iftype == IRLIM) {
            toamph(s->y, s->x, s->h->npts, s->y, s->x);
            lconv = TRUE;
            ispectype = IAMPH;
        } else if (cmsam.lwrlim && s->h->iftype == IAMPH) {
            torlim(s->y, s->x, s->h->npts, s->y, s->x);
            lconv = TRUE;
            ispectype = IRLIM;
        } else {
            lconv = FALSE;
        }

        /* -- Determine character length of output file name. */

        if (cmsam.lwspov) {
            tmp = s->m->filename;
            strlcpy(prefix, tmp, sizeof(prefix));
        } else {
            tmp = string_list_get(list, jdfl - 1);
            strlcpy(prefix, tmp, sizeof(prefix));
        }

        /* -- Determine suffixes if KWSPTP is 'ASIS'. */
        if (strcmp(kmsam.kwsptp, "ASIS    ") == 0) {
            if (ispectype == -1) {
                ispectype = s->h->iftype;
            }
            if (ispectype == IRLIM) {
                strcpy(kmsam.kwsps1, ".rl");
                strcpy(kmsam.kwsps2, ".im");
            } else {
                strcpy(kmsam.kwsps1, ".am");
                strcpy(kmsam.kwsps2, ".ph");
            }
        }

        /* -- Adjust header for writes. */

        nfreq = s->h->npts / 2 + 1;
        s->h->npts = nfreq;
        sac_set_float(s, SAC_B, 0.0);
        s->h->iftype = IXY;
        sac_be(s);

        /* -- Write first spectral component if requested. */
        if (cmsam.lwspc1) {
            snprintf(kwspnm, sizeof(kwspnm), "%s%s", prefix, kmsam.kwsps1);
            sac_write_r(s, kwspnm, TRUE, FALSE, nerr);
            if(*nerr) {
                goto L_8888;
            }
        }

        /* -- Write second spectral component if requested. */

        if (cmsam.lwspc2) {
            float *ytmp = s->y;
            s->y = s->x;
            snprintf(kwspnm, sizeof(kwspnm), "%s%s", prefix, kmsam.kwsps2);
            sac_write_r(s, kwspnm, TRUE, FALSE, nerr);
            s->y = ytmp;
            if(*nerr) {
                goto L_8888;
            }
        }

        /* -- Convert file back to original type if necessary. */

        if (lconv && ispectype == IRLIM) {
            toamph(s->y, s->x, s->h->npts, s->y, s->x);
            ispectype = IAMPH;
        } else if (lconv && ispectype == IAMPH) {
            torlim(s->y, s->x, s->h->npts, s->y, s->x);
            ispectype = IRLIM;
        }

    }

  L_8888:
    return;

        /*=====================================================================
	 * MODIFICATION HISTORY:
	 *    910417:  Transfter iftype value to ispectype, immed. after vfspec
	 *             to rid bug caused by "iftype=ixy" before component write
	 *    850328:  Added call to ZPUTC to copy char. info before writing.
	 *    821122:  Deleted use of temporary arrays.
	 *    820927:  Mod due to changes in HDR storage format.
	 *    820331:  Combined "parse" and "control" modules.
	 *    810327:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850124
	 *===================================================================== */

}                               /* end of function */
