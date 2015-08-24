#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <libgen.h>
#endif

#include "amf.h"
#include "sss.h"
#include "dfm.h"
#include "bool.h"

#include "msg.h"
#include "clf.h"
#include "cpf.h"

SSS_EXTERN


void /*FUNCTION*/
xliststack(nerr)
     int *nerr;
{
    char kline[MCMSG + 1], kpol[9];
    int j, jdfl, jvm, jvm_;
    static char kvapp[9] = " VAPP = ";
    static char kt0vm[9] = " T0VM = ";
    static char kvappi[9] = "VAPPI = ";
    static char kt0vmi[9] = "T0VMI = ";
    static char kdvm[9] = "  DVM = ";
    static char ktvm[9] = "  TVM = ";
    static char kcalc[9] = " (CALC) ";
    static char kinput[9] = " (INPUT)";
    static char koff[9] = "OFF     ";
    static char knmo[9] = "= NMO   ";
    static char krefr[9] = "= REFR  ";
    char *tmp;
    sac *s;

        /*=====================================================================
	 * PURPOSE:  To execute the LISTSTACK command.
	 *           This command lists the current stack list properties.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  sss/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    MCMSG
	 *    dfm:     ndfl, kdfl
	 *    sss:     lvm, lpol, wt, dlyt, dlyn, dlyvm, dst, dlyti, dlyni, twlim,
	 *             ivm, inmo, irefr, lcvapp, vapp, vappi, tovm, dvm, tvm,
	 *	       beginTime, endTime
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    sss:     lnarli
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, lclog2, cfmt, cresp, vmcalc, vmdly, 
	 *             autooutmsg
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    960708:  Added begin time and end time. maf
	 *    890105:  Changed from terminal output to message subsystem.
	 *    850821:  Major revision of subprocess.
	 *    791025:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850821
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    /* PARSING PHASE: */

    /* - Loop on each token in command: */

  L_1000:
    if (lcmore(nerr)) {

        /* -- "NARROW|WIDE":  change width of output. */
        if (lclog2("NARROW$", 8, "WIDE$", 6, &cmsss.lnarli)) {

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

    /* EXECUTION PHASE: */

    /* - Activate automatic output message mode. */

    autooutmsg(TRUE);
    setmsg("OUTPUT", 99);

    /* - Compute velocity model delays for listing. */

    if (Lvm[1]) {
        vmcalc(1, nerr);
        if (*nerr != 0)
            goto L_8888;
        vmdly(nerr);
        if (*nerr != 0)
            goto L_8888;
    }

    /* - Write column headings.  
       Rearranged 960708 to fit begin time and end time, maf */

    if (cmsss.lnarli) {
        mprint(" filename  weight      delayt\
      delayn     delayvm   polarity   distance");
        mprint("                           delayti\
     delayni      begin       end");
    } else {
        mprint(" filename                  weight      delayt\
      delayn     delayvm    polarity    distance     delayti     delayni\
      begin       end");
    }

    /* - Loop on stack list. */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        tmp = basename(s->m->filename);
        if (tmp) {
            strcpy(kpol, "  NORMAL");
            if (!Lpol[jdfl])
                strcpy(kpol, "REVERSED");
            if (cmsss.lnarli) {
                /* Rearranged 960708 to fit begin time and end time, maf */
                mprint(" %s  %12.3f%12.3f%12.3f%12.3f %s %12.3f", tmp,
                       Wt[jdfl], Dlyt[jdfl], Dlyn[jdfl], Dlyvm[jdfl], kpol,
                       Dst[jdfl]);
                mprint("                      %12.3f%12.3f%12.3f%12.3f",
                       Dlyti[jdfl], Dlyni[jdfl], Tbegin[jdfl], Tend[jdfl]);
            } else {
                /* Rearranged 960708 to fit begin time and end time, maf */
                mprint(" %20s%12.3f%12.3f%12.3f%12.3f    %8s%12.3f%12.3f%12.3f%12.3f%12.3f",
                        tmp, Wt[jdfl], Dlyt[jdfl], Dlyn[jdfl], Dlyvm[jdfl],
                        kpol, Dlyti[jdfl], Dlyni[jdfl], Dst[jdfl], Tbegin[jdfl],
                        Tend[jdfl]);
            }
        } else {
            printf("programming logic error-xliststack\n");
            return;
        }
    }

    /* - Write time window. */

    mprint(" Time Window:%12.3f%12.3f", Twlim[1], Twlim[2]);

    /* - Write velocity models. */

    for (jvm = 1; jvm <= MVM; jvm++) {
        jvm_ = jvm - 1;

        if (Lvm[jvm]) {
            if (Ivm[jvm] == cmsss.inmo) {
                mprint(" Stack Velocity Model %1d %8s", jvm, knmo);
            } else if (Ivm[jvm] == cmsss.irefr) {
                mprint(" Stack Velocity Model %1d %8s", jvm, krefr);
            }
            if (Lcvapp[jvm]) {
                mprint("    %8s%12.8f    %8s", kvapp, Vapp[jvm], kcalc);
            } else {
                mprint("    %8s%12.8f    %8s", kvapp, Vapp[jvm], kinput);
            }
            if (Vappi[jvm] != 0.) {
                mprint("    %8s%12.8f", kvappi, Vappi[jvm]);
            }
            if (Lct0vm[jvm]) {
                mprint("    %8s%12.8f    %8s", kt0vm, T0vm[jvm], kcalc);
            } else {
                mprint("    %8s%12.8f    %8s", kt0vm, T0vm[jvm], kinput);
            }
            if (T0vmi[jvm] != 0.) {
                mprint("    %8s%12.8f", kt0vmi, T0vmi[jvm]);
            }
            sprintf(kline, "    %8s", kdvm);
            for (j = 1; j <= Ndvm[1]; j++) {
                sprintf(kline + 12 + ((j - 1) * 12), "%12.8f",
                        cmsss.dvm[jvm_][j - 1]);
            }
            mprint(kline);
            sprintf(kline, "    %8s", ktvm);
            for (j = 1; j <= Ntvm[1]; j++) {
                sprintf(kline + 12 + ((j - 1) * 12), "%12.8f",
                        cmsss.tvm[jvm_][j - 1]);
            }
            mprint(kline);
        } else {
            mprint(" Stack Velocity Model %1d %8s", jvm, koff);
        }

    }

    /* - Deactivate automatic output message mode. */

    autooutmsg(FALSE);

    /* - Format statements. */

  L_8888:
    return;

}                               /* end of function */
