/** 
 * @file   xrtab.c
 * 
 * @brief  Read a Table file
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dfm.h"
#include "bool.h"
#include "hdr.h"
#include "com.h"
#include "gdm.h"
#include "gem.h"
#include "amf.h"

#include "clf.h"

#include "string_utils.h"

#include "wild.h"
#include "pl.h"
#include "bot.h"
#include "ucf.h"
#include "ssi.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"
#include "debug.h"

#include <fstr.h>

GEM_EXTERN

GDM_EXTERN

#define	MALPHA	100
#define	MAXCH	40
#define	MBLKSZ	500
#define	MENTRY	40

DFM_EXTERN

struct t_xracom {
    int nhlines;
} xracom;

/** 
 * Execute the command READTABLE wich reads a general alphanumeric data file
 * 
 * @param lplot 
 *    Whether to plot an alpha file
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   970129:  Add parameter (1) to cnvfre.  1 means that if a string
 *                  of digits is too long, warn the user and end the 
 *                  command.  maf 
 * @date   920501:  Added test to clear memory on NOFILES and MEMORY delete
 *                  READERR contition.
 * @date   920429:  Added lfilesok test and nsndfl to save current file count.
 * @date   920319:  Bug fix. Changed "more=.false." to "lmore.eq.false"
 * @date   920226:  Added data-set storage logic.
 * @date   911009:  Added more descriptive error message when potential
 *                  formatting error occurs (Error = 0002) in cmvfre or
 *                  cmvfmt.
 * @date   870722:  Added descriptive error messages.
 * @date   860910:  Original version.
 *
 */
void
xrtab(int lplot, int *nerr) {

    char slash[2], kalpha[MALPHA][21], kcard[MCMSG + 1], kcont[MCMSG + 1],
        kform[MCMSG + 1];
    int lbcksp, lexpnd, lfree, lmore, ltoend, numchar, lprint = FALSE, ltry =
        FALSE;
    int idx, ic, ic1, ic2, iopch[MAXCH], ipt = 0, itype, jdx, jch, jen, nc, nchar,
        ndcont, ndflsv, ndform, nentry, nlnch[MAXCH - (0) + 1],
        nptch[MAXCH - (0) + 1], nsndfl, numch, numxch, numych;
    FILE *nun;
    float fentry[MENTRY];
    float *y[MAXCH];
    sac *s;
    static string_list *last_list = NULL;
    int i;
    char *tmp, *file;
    string_list *list, *files;
    static int _aini = 1;
    char *cattemp;

    int *const Iopch = &iopch[0] - 1;

    if (!last_list) {
        last_list = string_list_init();
    }

    list = NULL;

    if (_aini) {                /* Do 1 TIME INITIALIZATIONS! */
        xracom.nhlines = 0;
        _aini = 0;
    }

    /* initialize character arrays */
    for (idx = 0; idx < MCMSG; idx++) {
        kcard[idx] = kcont[idx] = kform[idx] = ' ';
    }
    kcard[MCMSG] = kcont[MCMSG] = kform[MCMSG] = '\0';

    *nerr = 0;

    /* PARSING PHASE: */
    lmore = FALSE;
    ndflsv = 0;

    /* - Loop on each token in command: */
    while (lcmore(nerr)) {

        /* -- "MORE":  signifies addition of more files to current read 
         *             filelist rather than replacement of current list
         *             with new one. */
        if (lckey("&MORE$", 7)) {
            lmore = TRUE;
            ndflsv = saclen();
        }

        /* -- "DIR CURRENT|name":  set name of the default subdirectory. */
        else if (lkchar("DIR#$", 6, MCPFN, kmdfm.krddir, MCPFN + 1, &nchar)) {
            if (memcmp(kmdfm.krddir, "CURRENT", 7) == 0 ||
                memcmp(kmdfm.krddir, "current", 7) == 0) {
                fstrncpy(kmdfm.krddir, MCPFN, " ", 1);
            } else if (kmdfm.krddir[nchar - 1] != KDIRDL) {
                slash[0] = KDIRDL;
                slash[1] = '\0';
                subscpy(kmdfm.krddir, nchar, -1, MCPFN, slash);
            }
        }

        /* -- "HEADER count": set number of lines to skip. */
        else if (lkint("HEADER$", 8, &xracom.nhlines)) {        /* do nothing */
        }

        /* -- "CONTENT string": set default content. */
        else if (lkchar
                 ("&CONTENT$", 10, MCMSG, kmdfm.kdcont, MCMSG + 1, &ndcont)) {
            modcase(TRUE, kmdfm.kdcont, ndcont, kmdfm.kdcont);
        }

        /* -- "FREE":  use free-field rather than formatted input. */
        else if (lckey("&FREE$", 7))
            cmdfm.ldfree = TRUE;

        /* -- "FORMAT string": use formatted input & set default format. */
        else if (lkchar("&FORMAT$", 9, MCMSG, kmdfm.kdform, MCMSG + 1, &ndform)) {
            cmdfm.ldfree = FALSE;
            modcase(TRUE, kmdfm.kdform, ndform, kmdfm.kdform);
        }

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

        /* if PRINT option is tried, get printer name */
        else if (lplot && ltry) {
            char command[81];
            lcchar(kmgem.kptrName, sizeof(kmgem.kptrName));

            sprintf(command, "lpstat -v | grep %s", kmgem.kptrName);
            if (system(command)) {
                //cmcom.jcom-- ;
                kmgem.kptrName[0] = '\0';
            }

            if (!lprint)
                kmgem.kptrName[0] = '\0';

            ltry = FALSE;
        }

        /* -- "PRINT":  print the final product */
        else if (lplot && lckey("PRINT#$", 8)) {
            ltry = TRUE;
            if (cmgdm.lbegf) {
                setmsg("WARNING", 2403);
                outmsg();
                clrmsg();
            } else {
                lprint = TRUE;
            }
        }

        /* -- TRUST:  whether or not to trust matching evids while moving
           data from SAC buffers to CSS buffers. */
        else if (lklog("TRUST#$", 8, &cmdfm.ltrust)) {  /* do nothing */
        }

        /* -- "filelist":  define a new filelist (or add to old filelist) */
        else if ((list = lcdfl())) {    /* do nothing */
        }

        /* -- Bad syntax. */
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }

    if (*nerr != 0)
        goto L_8888;

    if (!list) {
        list = string_list_init();
        string_list_extend(list, last_list);
    }

    /* EXECUTION PHASE: */
    /* - Commit or rollback data according to lmore and cmdfm.icomORroll */
    if (lmore) {
        alignFiles(nerr);
        if (*nerr)
            return;
        cmdfm.nfilesFirst = saclen();
    } /* end if */
    else
        cmdfm.nfilesFirst = 0;

    /* - Echo expanded file list if requested. */
    /* - Save current file count of wroking storage, incase we can't read
     *   the requested file. */

    nsndfl = saclen();

    /* - Set the current file count for using read or read-more. */

    cmdfm.ndfl = ndflsv;

    files = wildfl(kmdfm.krddir, MCPFN + 1, list, &lexpnd);
    if (cmdfm.lechof && lexpnd) {
        setmsg("OUTPUT", 0);
        ic1 = 0;
        for (i = 0; i < string_list_length(files); i++) {
            tmp = string_list_get(files, i);
            apcmsg2(tmp, strlen(tmp) + 1);
        }
        wrtmsg(MUNOUT);
    }

    /* -- Test to see that at least one file can be opened and read.
     *    If not, resore current file count of working storage. */

    if (!lfilesok(files, NULL, 0, lmore, FALSE, FALSE, nerr)) {
        cmdfm.ndfl = nsndfl;

        /* --- If destroying files in memory, */
        if (strcmp(kmdfm.kecmem, "DELETE  ") == 0)
            sacclear();

        *nerr = 1301;
        goto L_8888;
    }

    /* - Release all old memory blocks and initialize data file list
     *   unless MORE option is on. */

    /* -- Release all memory blocks. */

    if (!lmore) {
        sacclear();
        if (*nerr != 0)
            goto L_8888;
    }

    /* - Main loop on each input file: */

    for (i = 0; i < string_list_length(files); i++) {
        file = string_list_get(files, i);

        /* -- Open input alphanumeric data file. */
        zopens(&nun, file, strlen(file), nerr);
        if (*nerr != 0)
            goto L_8888;

        /* -- Read first card.
         * --- If is a description card, decode it into CONTENT and FORMAT
         information.
         * --- Otherwise, backspace input file. */
        strcpy(kcont, kmdfm.kdcont);
        nc = indexb(kmdfm.kdform, MCMSG + 1);
        if (kmdfm.kdform[0] == '(' && kmdfm.kdform[nc - 1] == ')') {
            fstrncpy(kform, MCMSG, kmdfm.kdform, nc);
        } else {
            fstrncpy(kform, MCMSG, "(", 1);
            fstrncpy(kform + 1, MCMSG - 1, kmdfm.kdform, nc);
            fstrncpy(kform + 1 + nc, MCMSG - 1 - nc, ")", 1);
        }
        lfree = cmdfm.ldfree;

        if (fgetsp(kcard, MCMSG + 1, nun) == NULL) {
            if (feof(nun))
                goto L_5000;
            else
                goto L_9000;
        }
        if (kcard[(numchar = strlen(kcard) - 1)] == '\n')
            kcard[numchar] = '\0';

        /* replace tabs with blanks */
        for (idx = 0; idx < numchar; idx++)
            if (kcard[idx] == '\t')
                kcard[idx] = ' ';

        nc = indexb(kcard, MCMSG + 1);
        ic = 0;
        upcase(kcard, nc, kcard, MCMSG + 1);
        poptok(kcard, nc, &ic, &ic1, &ic2, &itype);
        lbcksp = TRUE;

        while (1) {
            if (memcmp(kcard + ic1 - 1, "CONTENT", 7) == 0) {
                poptok(kcard, nc, &ic, &ic1, &ic2, &itype);
                if (itype > 0)
                    fstrncpy(kcont, MCMSG, kcard + ic1 - 1, ic2 - ic1 + 1);
                lbcksp = FALSE;
                poptok(kcard, nc, &ic, &ic1, &ic2, &itype);
            } else if (memcmp(kcard + ic1 - 1, "FREE", 4) == 0) {
                lfree = TRUE;
                break;
            } else if (memcmp(kcard + ic1 - 1, "FORMAT", 6) == 0) {
                lfree = FALSE;
                poptok(kcard, nc, &ic, &ic1, &ic2, &itype);
                if (itype > 0) {
                    if (kcard[ic1 - 1] == '(' && kcard[ic2 - 1] == ')') {
                        fstrncpy(kform, MCMSG, kcard + ic1 - 1, ic2 - ic1 + 1);
                    } else {
                        fstrncpy(kform, MCMSG, "(", 1);
                        fstrncpy(kform + 1, MCMSG - 1, kcard + ic1 - 1,
                                 ic2 - ic1 + 1);
                        fstrncpy(kform + 1 + (ic2 - ic1 + 1),
                                 MCMSG - 1 - (ic2 - ic1 + 1), ")", 1);
                    }
                }
                lbcksp = FALSE;
                poptok(kcard, nc, &ic, &ic1, &ic2, &itype);
            } else if (lbcksp) {
                backspace(nun, 1L);
                break;
            }
        }                       /* end while (1) */

        /* -- Skip over header lines */
        for (idx = 1; idx <= xracom.nhlines; idx++) {
            if (fgetsp(kcard, MCMSG + 1, nun) == NULL) {
                if (feof(nun))
                    goto L_5000;
                else
                    goto L_9000;
            }
        }

        /* -- Determine number of entries per card if formatted option. */
        if (!lfree)
            detnum(kform, MCMSG + 1, &nentry);

        /* -- Decode content information. */
        decont(kcont, MCMSG + 1, MAXCH, &numch, &numxch, &numych, iopch,
               &ltoend, nerr);
        if (*nerr != 0)
            goto L_8888;

        /* -- Prime it to read in first card to see how many
           entries are really there */
        do {
            if (fgetsp(kcard, MCMSG + 1, nun) == NULL) {
                if (feof(nun))
                    goto L_5000;
                else
                    goto L_9000;
            }
            if (kcard[(numchar = strlen(kcard) - 1)] == '\n')
                kcard[numchar] = '\0';

            /* replace tabs with blanks */
            for (idx = 0; idx < numchar; idx++)
                if (kcard[idx] == '\t')
                    kcard[idx] = ' ';

            for (idx = 0; idx < MALPHA; idx++) {
                memset(kalpha[idx], ' ', 21);
            }

            /* --- Convert card to list of floating point values.
             *     Each card is in either free field or formatted. */
            if (lfree) {
                cnvfre(kcard, MCMSG + 1, MENTRY, &nentry, fentry, iopch,
                       (char *) kalpha[0], 21, 1, nerr);
            } else {
                cnvfmt(kcard, MCMSG + 1, kform, MCMSG + 1, nentry, fentry,
                       nerr);
            }

            /* -- Loop on each line in text file.
             * --- Say something descriptive about data problems and get out. */
            if (*nerr != 0) {
                setmsg("WARNING", *nerr);
                apcmsg("Trouble reading line. Skipping...", 34);
                apcmsg(kcard, MCMSG + 1);
                outmsg();
            }
        } while (*nerr);

        /* --- Redetermine how many y channels there really are */
        for (idx = 1; idx <= nentry; idx++) {
            if (Iopch[idx] > 0)
                numych = Iopch[idx];
        }

        /* -- Allocate memory block for x channel if present.
         *    This is treated as channel 0 in the storage scheme. */
        if (numxch == 1) {
            nlnch[0] = MBLKSZ;
            nptch[0] = 0;
            y[0] = (float *) malloc(sizeof(float) * nlnch[0]);
            if (!y[0])
                goto L_8888;
        } else if (numxch > 1) {
            *nerr = 1361;
            setmsg("ERROR", *nerr);
            apcmsg("Can only have one X channel per file.", 38);
            goto L_8888;
        }

        /* -- Allocate memory blocks for y channel(s). */
        if (numych >= 1) {
            for (jdx = 1; jdx <= numych; jdx++) {
                nlnch[jdx] = MBLKSZ;
                nptch[jdx] = 0;
                y[jdx] = (float *) malloc(sizeof(float) * nlnch[jdx]);
                if (!y[jdx])
                    goto L_8888;
            }
        } else {
            *nerr = 1362;
            setmsg("ERROR", *nerr);
            goto L_8888;
        }

        /* --- Say something descriptive about data problems and get out. */
      L_4000:
        if (*nerr != 0) {
            setmsg("WARNING", *nerr);
            apcmsg("Trouble reading line. Skipping...", 34);
            apcmsg(kcard, MCMSG + 1);
            outmsg();
        }

        /* --- Fill in the channels, according to the the content vector. */
        else {
            for (jen = 1; jen <= nentry; jen++) {
                jch = Iopch[jen];
                if (jch >= 0) {
                    if (nptch[jch] >= nlnch[jch]) {
                        float *tmp;
                        nlnch[jch] *= 2;
                        tmp = (float *) malloc(sizeof(float) * nlnch[jch]);
                        copy_float(y[jch], tmp, nptch[jch]);
                        FREE(y[jch]);
                        y[jch] = tmp;
                    }
                    nptch[jch] = nptch[jch] + 1;
                    ipt = nptch[jch] + 1;
                    y[jch][nptch[jch] - 1] = fentry[jen - 1];
                }
            }
        }
        /* -- Read in next card. */
        if (fgetsp(kcard, MCMSG + 1, nun) == NULL) {
            if (feof(nun))
                goto L_5000;
            else
                goto L_9000;
        }
        if (kcard[(numchar = strlen(kcard) - 1)] == '\n')
            kcard[numchar] = '\0';

        /* replace tabs with blanks */
        for (idx = 0; idx < numchar; idx++)
            if (kcard[idx] == '\t')
                kcard[idx] = ' ';

        /* --- Convert card to list of floating point values.
         *     Each card is in either free field or formatted. */
        if (lfree) {
            if (ipt >= MALPHA)
                ipt = MALPHA;

            cnvfre(kcard, MCMSG + 1, MENTRY, &nentry, fentry, iopch,
                   (char *) kalpha[ipt - 1], 21, 1, nerr);
        } else {
            cnvfmt(kcard, MCMSG + 1, kform, MCMSG + 1, nentry, fentry, nerr);
        }
        /* --- Loop on each card in input file. */
        goto L_4000;

        /* -- Come to here on end-of-file. */

      L_5000:
        /* -- Close input file. */
        zcloses(&nun, nerr);
        if (*nerr != 0)
            goto L_8888;

        for (jch = 1; jch <= numych; jch++) {
            s = sac_new();
            s->h->npts = nptch[jch];
            s->h->iftype = ITIME;
            s->h->leven = (numxch == 0);
            sac_alloc(s);
            copy_float(y[jch], s->y, s->h->npts);
            if (numxch == 0) {
                sac_set_float(s, SAC_DELTA, 1.0);
                sac_set_float(s, SAC_B, 0.0);
                sac_be(s);
            } else {
                copy_float(y[0], s->x, s->h->npts);
                sac_set_float(s, SAC_DELTA, SAC_FLOAT_UNDEFINED);
                sac_be(s);
            }
            extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax,
                   &s->h->depmen);
            if (numych == 1) {
                s->m->filename = fstrdup(file, strlen(file));
            } else {
                cattemp = malloc(strlen(file) + 2 + 1);
                sprintf(cattemp, "%s%02d", file, jch);
                s->m->filename = cattemp;
            }
            sacput(s);
        }

    }

    /* - Check again for a non-null DFL. */
    if (saclen() <= 0) {
        *nerr = 1301;
        setmsg("ERROR", *nerr);
    }

    /* -- UPDATE DATA-SET STORAGE - END */
    /* - Calculate range of dependent variable for use during plotting. */
    setrng();

    /* - Make a plot if requested. */
    if (lplot)
        plalpha((char *) kalpha, 21, MALPHA, lprint, nerr);
    /* - Return. (Try to close alphanumeric data file just to be sure.) */

    string_list_clear(last_list);
    string_list_extend(last_list, list);
    string_list_free(list);
    list = NULL;

  L_8888:
    if (*nerr == 0) {
        cmdfm.nreadflag = LOW;
        cmdfm.lread = TRUE;
        sacToSeisMgr(!lmore, FALSE, TRUE, nerr);
        cmdfm.lread = FALSE;
    }
    return;

  L_9000:
    *nerr = 114;
    setmsg("ERROR", *nerr);
    apcmsg2(file, strlen(file) + 1);
    goto L_8888;

}
