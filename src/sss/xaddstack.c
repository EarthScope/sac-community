#include <math.h>

#include "sss.h"
#include "dfm.h"
#include "hdr.h"
#include "bool.h"

#include "amf.h"
#include "bot.h"
#include "msg.h"
#include "ssi.h"
#include "cpf.h"
#include "dff.h"


SSS_EXTERN

DFM_EXTERN

void
xaddstack(int *nerr) {
    char kfile[MCPFN + 1];
    double delay;
    double tmp;
    /* The following add 960701 to correct a bug with Dst and to 
       incorporate begin time and end time. maf 
       Each is initialized FALSE but is set to TRUE if the 
       corresponding data variable is specified by the user. */
    int lDistanceDefined = FALSE, lBeginDefined = FALSE, lEndDefined = FALSE;

    sac *s;
        /*=====================================================================
	 * PURPOSE: To parse the parameter-setting command ADDSTACK.
	 *          This command adds one or more files to the signal stack.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001, 5108.
	 *=====================================================================
	 * MODULE/LEVEL:  sss/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    mcpfn
	 *    sss:     dlyng, dlytg, dlynig, dlytig, wtg, dstg, lpolg, srcfac
	 *    hdr:     itime, ixy, fundef, dist, begin, ennd
	 *    mem:     sacmem
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    dfm:     ndfl, kdfl
	 *    sss:     dlyn, dlyt, dlyni, dlyti,
	 *             wt, dst, lpol, del, beginTime, endTime
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, cfmt, cresp, lcchar, lkreal, lklog2, lkchar,
	 *             setmsg, apcmsg, rdsac
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    kfile:   Name of file to add to stack. [c]
	 *    nlen:    Number of points in file. [i] {NOT USED}
	 *    ndxh:    Index in sacmem of header. [i] {NOT USED}
	 *    ndx1:    Index in sacmem of first data component. [i] {NOT USED}
	 *    ndx2:    Index in sacmem of second data component. [i] {NOT USED}
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    960701:  Added begin time and end time.
	 *    881117:  Fixed bug with data file list counter when an error occurred.
	 *             Changes due to restructuring data file access methods.
	 *    850812:  Major rewrite of subprocess.
	 *    821130:  Changed to new command parsing logic.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850812
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    /* - Parse position dependent tokens.
     *   (This is the name of the file to be added to the stack.) */

    if (lcchar(kfile, sizeof(kfile))) {

        /* -- Start with global property values. */
        Lincl[saclen()] = TRUE;
        Dlyn[saclen()] = cmsss.dlyng;
        Dlyt[saclen()] = cmsss.dlytg;
        Dlyni[saclen()] = cmsss.dlynig;
        Dlyti[saclen()] = cmsss.dlytig;
        Wt[saclen()] = cmsss.wtg;
        Lpol[saclen()] = cmsss.lpolg;
    }

    /* - Loop on rest of tokens in command:
     *   (These are keywords which change properties for this file only.) */

    while (lcmore(nerr)) {

        /* -- "WEIGHT v":  define global weight property. */
        if (lkreal("WEIGHT$", 8, &tmp)) {
            Wt[saclen()] = (float) tmp;
        }

        /* -- "DELAY v":  define global static delay propertys. */
        else if (lkreal("DE#LAY$", 8, &delay)) {
            if (lckey("SECONDS$", 9)) {
                Dlyt[saclen()] = delay;
            } else if (lckey("POINTS$", 8)) {
                Dlyn[saclen()] = delay;
            }
        }

        /* -- "INCREMENT v":  define global static delay propertys. */
        else if (lkreal("INCREMENT$", 11, &delay)) {
            if (lckey("SECONDS$", 9)) {
                Dlyti[saclen()] = delay;
            } else if (lckey("POINTS$", 8)) {
                Dlyni[saclen()] = delay;
            }
        }

        /* -- "NORMAL/REVERSED":  define global polarity property. */
        else if (lclog2("NORMAL$", 8, "REVERSED$", 10, &Lpol[saclen()])) {      /* do nothing */
        }

        /* -- "DISTANCE v":  define global distance property. */
        else if (lkreal("DI#STANCE$", 11, &tmp)) {
            lDistanceDefined = TRUE;
            Dst[saclen()] = (float) tmp;
        }

        /* -- "BEGINTIME v":  define global begin time property. */
        else if (lkreal("BE#GINTIME$", 12, &tmp)) {
            lBeginDefined = TRUE;
            Tbegin[saclen()] = (float) tmp;
        }

        /* -- "ENDTIME v":  define global end time property. maf 960701 */
        else if (lkreal("END#TIME$", 10, &tmp)) {
            lEndDefined = TRUE;
            Tend[saclen()] = (float) tmp;
        }

        /* -- Bad syntax. */
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }

    /* - The above loop is over when one of two conditions has been met.:
     *   (1) An error in parsing has occurred.  In this case NERR is > 0.
     *   (2) All the tokens in the command have been successfully parsed. */

    /* CHECKING PHASE: */

    /* - Read file. */

    rstrip(kfile);
    s = sac_read(kfile, nerr);
    if(*nerr) {
        goto L_8888;
    }
    sacput(s);

    if (!s->h->leven) {
        *nerr = 1306;
        setmsg("ERROR", *nerr);
        apcmsg(kfile, MCPFN + 1);
        goto L_8888;
    } else if (s->h->iftype != ITIME && s->h->iftype != IXY) {
        *nerr = 1307;
        setmsg("ERROR", *nerr);
        apcmsg(kfile, MCPFN + 1);
        goto L_8888;
    } else if (saclen() > 1) {
        if (fabs(DT(s) - cmsss.del) > cmsss.srcfac && cmsss.lsrc) {
            *nerr = 5109;
            setmsg("ERROR", *nerr);
            goto L_8888;
        }
    } else {
        cmsss.del = DT(s);
    }

    /* If distance was not specified by the user, maf 960701 */
    if (!lDistanceDefined) {
        /* -- Use global distance or distance from header. */
        if (cmsss.dstg != SAC_FLOAT_UNDEFINED) {
            Dst[saclen()] = cmsss.dstg;
        } else if (s->h->dist != SAC_FLOAT_UNDEFINED) {
            Dst[saclen()] = s->h->dist;
        } else {
            Dst[saclen()] = SAC_FLOAT_UNDEFINED;
        }
    }
    /* end if ( !lDistanceDefined ) */
    if (!lBeginDefined) {       /* if begin time not user specified, maf 960701 */
        Tbegin[saclen()] = B(s);
    }
    /* end if ( !lBeginDefined ) */
    if (!lEndDefined) {         /* if end time not user specified, maf 960701 */
        Tend[saclen()] = E(s);
    }
    /* end if ( !lEndDefined ) */
    if (!*nerr) {
        if (cmdfm.nreadflag != LOW) {
            if (cmdfm.ltrust)
                cmdfm.nreadflag = HIGH;
            else
                cmdfm.nreadflag = LOW;
        }
        cmdfm.nfilesFirst = 0;
        cmdfm.lread = TRUE;
        sacToSeisMgr(TRUE, FALSE, TRUE, nerr);
        cmdfm.lread = FALSE;
    }

    /* - Decrement the file list counter before returning if an error occurred. */

  L_8888:
    return;

}                               /* end of function */
