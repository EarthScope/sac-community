/** 
 * @file   xlh.c
 * 
 * @brief  List Header Variables
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dfm.h"
#include "bool.h"
#include "lhf.h"
#include "select.h"

#include "bot.h"
#include "exm.h"
#include "msg.h"
#include "gdm.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "datafilelist.h"
#include "dff.h"
#include "amf.h"
#include "errors.h"

HDR_EXTERN
LHF_EXTERN

char 
lh_prompt() {
    char kresp[9];
    outmsg();
    clrmsg();
    setmsg("OUTPUT", 99);
    zgpmsg("Waiting $", 10, kresp, 9);
    return kresp[0];
}

#define handle_lh_input()                       \
    do {                                        \
        nlw = 0;                                \
        switch(lh_prompt()){                    \
        case 'K': case 'k':                     \
        case 'Q': case 'q':                     \
            goto DONE;                          \
            break;                              \
        case 'G': case 'g':                     \
            lwait = FALSE;                      \
            break;                              \
        case 'N': case 'n':                     \
            goto NEXT_FILE;                     \
            break;                              \
        }                                       \
    } while(0)


/** 
 * Execute the command LISTHDR (LH) which lists header values
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   970425:  Fix bug so display fits in the window.  maf
 * @date   970129:  Print file number (jdfl).  maf
 * @date   961212:  All of the header variables are now in the default list.
 *                  Added INCLUSIVE option to show headers whether they are
 *                  defined or not.  maf
 * @date   900507:  Fixed bug with an odd number of items being listed with
 *                  the two-column output option. (VAX/VMS bug fix.)
 * @date   890104:  Now sending output to message handling system.
 * @date   860930:  Added a wait mechanism after each full screen.
 * @date   841026:  Extensive modifications made to entire subroutine.
 * @date   820806:  Changed to newest set of parsing and checking functions.
 *                  Updated line formatting using F77 character constructs.
 * @date   820119:  Fixed bug in listing KHDR values.
 * @date   811029:  Changed floating point output to G8.1 format.
 * @date   810528:  Added option to list only first file in dfl.
 * @date   810223:  Added check for null data file list.
 * @date   810120:  Changed to output message retrieval from disk.
 *
 */
void
xlh(int *nerr) {

    char kerase[41], vals[MRPT][41], ktok[9];
    int lwait;
    int i;
    int j, j_, jdfl, jrpt, jsprpt, nc1, nc2, nc3, nc4,
        nctx[MRPT], nctxm, nferr, nlscrn, nlw, nv;

    static int iform = 1;
    static char kblank[41] = "                                        ";
    int idx, ldef;
    char *tmp;
    sac *s;
    *nerr = 0;
    ldef = FALSE;
    for (idx = 0; idx < 8; idx++)
        ktok[idx] = ' ';
    ktok[8] = '\0';

    /* currently executing listhdr command. maf 961212 */
    cmhdr.llh = TRUE;

    jsprpt = 0;

    /* - Loop on each token in command: */
    while (lcmore(nerr)) {

        /* -- "DEFAULT/PICKS/SPECIAL":  change type of header report. */
        if (lclist((char *) kmlhf.krpttp, 9, cmlhf.nrpttp, &cmlhf.irpttp)) {
            if (cmlhf.irpttp == 1 || cmlhf.irpttp == 4) {
                for (j = 1; j <= cmlhf.nstrpt; j++) {
                    j_ = j - 1;
                    strcpy(kmlhf.krpt[j_], kmlhf.kstrpt[j_]);
                }
                cmlhf.nrpt = cmlhf.nstrpt;
            } else if (cmlhf.irpttp == 2) {
                for (j = 1; j <= cmlhf.npkrpt; j++) {
                    j_ = j - 1;
                    strcpy(kmlhf.krpt[j_], kmlhf.kpkrpt[j_]);
                }
                cmlhf.nrpt = cmlhf.npkrpt;
            } else if (cmlhf.irpttp == 3) {
                for (j = 1; j <= cmlhf.nsprpt; j++) {
                    j_ = j - 1;
                    strcpy(kmlhf.krpt[j_], kmlhf.ksprpt[j_]);
                }
                cmlhf.nrpt = cmlhf.nsprpt;
            }

        }

        /* -- "FILES ALL/nlist":  print all headers or only a subset. */
        else if (lckey("FILES#$", 8)) {
            if (lckey("ALL$", 5)) {
                cmlhf.lstall = TRUE;
            } else if (lckey("NONE$", 6)) {
                ldef = TRUE;
            } else if (lcia(1, saclen(), cmlhf.ilhlst, &cmlhf.nlhlst)) {
                cmlhf.lstall = FALSE;
            }
        }

        /* -- "INCLUSIVE": print headers even if they are undefined. */
        else if (lklog("INC#LUSIVE$", 12, &cmhdr.linc)) {       /* do nothing */
        }

        /* -- "COLUMNS n": change number of output columns. */
        else if (lkirc("COLUMNS#$", 10, 1, 2, &cmlhf.nlhcol)) { /* do nothing */
        }

        /* -- "FIRST": Obsolete keyword for first file only. */
        else if (lckey("FIRST#$", 8)) {
            cmlhf.lstall = FALSE;
            cmlhf.nlhlst = 1;
            cmlhf.ilhlst[0] = 1;
        }

        else if (lcchar(ktok, sizeof(ktok))) {
            if (jsprpt < MSPRPT) {
                jsprpt = jsprpt + 1;
                strcpy(kmlhf.ksprpt[jsprpt - 1], ktok);
                cmlhf.nrpt = jsprpt;
                strcpy(kmlhf.krpt[cmlhf.nrpt - 1], ktok);
            } else {
                *nerr = 1309;
                setmsg("ERROR", *nerr);
                apimsg(jsprpt);
            }
        } else {
            /* -- Bad syntax. */
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }

    if (*nerr != 0) {
        /* no longer executing xlh() */
        cmhdr.llh = FALSE;
        return;
    }

    /* - Save length of special report if needed. */
    if (jsprpt > 0)
        cmlhf.nsprpt = jsprpt;

    if (ldef) {
        /* no longer executing xlh(). */
        cmhdr.llh = FALSE;
        return;
    }

    /* CHECKING PHASE: */
    /* - Check for null data file list. */
    vflist(nerr);
    if (*nerr != 0) {
        /* no longer executing xlh(). */
        cmhdr.llh = FALSE;
        return;
    }

    /* EXECUTION PHASE: */

    /* - Get screen attributes (number of lines per screen and
     *   text to send to erase screen, if any.) */

    getalphainfo(&nlscrn, kerase, 41);
    if (nlscrn <= 0)
        nlscrn = 23;

    if (cmlhf.lstall) {
        setinputmode("ALL");
    } else {
        setinputmode("SELECT");
        selectinputfiles(cmlhf.ilhlst, cmlhf.nlhlst);
    }

    nlw = 0;
    lwait = gettextwait();
    autooutmsg(TRUE);
    setmsg("OUTPUT", 99);
    jdfl = 0;
 NEXT_FILE:
    if (nextinputfile(&jdfl)) {
        //getfil( jdfl, FALSE, &junk1, &junk2, &junk3, nerr );
        if (!(s = sacget(jdfl - 1, FALSE, nerr))) {
            autooutmsg(FALSE);
            /* no longer executing xlh(). */
            cmhdr.llh = FALSE;
            return;
        }
        if ((tmp = s->m->filename)) {
            mprint(" ");
            mprint(" FILE: %s - %d", tmp, jdfl);
            mprint("%.*s\n", strlen(tmp) + 6, 
                   "--------------------------------------------------");
            nlw = nlw + 4;
        }
        nv = 0;
        nctxm = 0;
        for (jrpt = 1; jrpt <= cmlhf.nrpt; jrpt++) {
            formhv((char *) kmlhf.krpt[jrpt-1], 9, iform,
                   (char *) vals[nv], 41, &nferr);
            if (nferr == 0) {
                nctx[nv] = strchr(vals[nv], '=') - vals[nv];
                nctxm = max(nctxm, nctx[nv]);
                nv = nv + 1;
            }
        }
        if (cmlhf.nlhcol == 1) {
            for (i = 0; i < nv; i++) {
                nc1 = 2 + nctxm - nctx[i];
                mprint(" %.*s %s", nc1, kblank, vals[i]);
                nlw = nlw + 1;
                if (lwait && (nlw >= (nlscrn - 2))) {
                    handle_lh_input();
                }
            }
        } else {
            strcpy(vals[nv], "                                        ");
            for (i = 0; i < nv; i += 2) {
                nc1 = 2 + nctxm - nctx[i];
                nc2 = indexb((char *) vals[i], 41);
                nc3 = 2 + nctxm - nctx[i + 1];
                nc4 = indexb((char *) vals[i + 1], 41);
                rstrip(vals[i]);
                rstrip(vals[i+1]);
                if (nc4 > 0) {
                    if ((nc1 + nc2) < 40) {
                        nc3 += 40 - (nc1+nc2);
                    }
                    mprint(" %*.s%s%*.s%s", nc1, kblank, vals[i],
                           nc3, kblank, vals[i+1]);
                } else {
                    mprint(" %*.s%s", nc1, kblank, vals[i]);
                }
                nlw = nlw + 1;
                if (lwait && (nlw >= (nlscrn - 1))) {
                    handle_lh_input();
                }
            }
        }

        /* -- Loop on entries in input dfl. */
        goto NEXT_FILE;
    }

    DONE:
    /* - Turn automatic output mode off before returning. */
    autooutmsg(FALSE);

    /* no longer executing xlh() */
    cmhdr.llh = FALSE;
    return;

}

void
lh_columns_set(int value) {
    if (value >= 1 && value <= 2) {
        cmlhf.nlhcol = value;
    }
}

void
lh_inclusive_set(int value) {
    cmhdr.linc = value;
}
