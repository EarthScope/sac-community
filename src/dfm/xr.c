/** 
 * @file   xr.c
 * 
 * @brief  Read
 * 
 */

#include <stdio.h>
#include <string.h>

#include "config.h"

#include "amf.h"
#include "dff.h"
#include "dfm.h"
#include "bool.h"
#include "SacHeader.h"
#include "errors.h"
#include "co.h"
#include "msg.h"
#include "ssi.h"
#include "cpf.h"
#include "clf.h"

#include "debug.h"

enum filetype {
    sacfi,
    alpha,
    xdr,
    segy
} ftype;

/** 
 * Execute the command READ (R) which reads data into memory. File into is 
 *    kept in data-set storage and is transferred to working storage
 *    when a data set is made active.
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_EXT_INTERFACE_NO_SPACE_LEFT
 *
 * @date   981103:  Removed TO options.  There are no datasets.  maf
 * @date   920508:  Added a space to the MORE and TO keyword options.
 * @date   920428:  Moved memory setup for data-set stuff to lfilesok.
 * @date   920403:  Test for max files already in sac's data-sets.
 * @date   910826:  Broke cleardfl into two routines, cleards for data-set 
 *                  storage and clearws for working-storage. Cleardfl calls 
 *                  both routines.
 * @date   910812:  Added option TO, in support of multiple data-sets.
 * @date   880308:  Added LDATA flag to call to READFL.
 * @date   870625:  Factored execution portion to readfl.
 *
 */
void
xr(int *nerr) {

    char dirDelimiter[2];
    int ldata;
    int lmore;
    int lscale;
    int lsdd;

    int nchar;
    static string_list *last_list = NULL;
    string_list *list;

    if (!last_list) {
        last_list = string_list_init();
    }

    ftype = sacfi;
    *nerr = 0;
    lmore = FALSE;
    list = NULL;

    /* PARSING PHASE: */
    /* - Parse position-dependent tokens: */

    while (lcmore(nerr)) {

        /* -- "MORE":  signifies addition of more files to current read
         *             filelist rather than replacement of current list
         *             with new one. */
        if (lckey("MORE#$", 8) && saclen() > 0) {
            lmore = TRUE;
        }

        /* -- "ALPHA":  input file is in alpha format. */
        else if (lckey("ALPHA#$", 9))
            ftype = alpha;

        /* -- "XDR":  input file is in xdr format. */
        else if (lckey("XDR#$", 7)) {
            ftype = xdr;
#ifndef HAVE_LIBRPC
            librpc_not_available();
            *nerr = 1301;
#endif /* ! HAVE_LIBRPC */
        }

        /* -- "SEGY":  input file is in segy format. */
        else if (lckey("SEGY#$", 8)) {
            ftype = segy;
        }

        /* -- "IO, IB": sets reference time, IB is default. */
        else if (lckey("IB#$", 6))
            cmdfm.iztype = IB;
        else if (lckey("IO#$", 6))
            cmdfm.iztype = IO;

        /* -- TRUST:  whether or not to trust matching evids while 
           moving data from SAC buffers to CSS buffers. */
        else if (lklog("TRUST#$", 8, &cmdfm.ltrust)) {  /* do nothing */
        }

        /* -- "DIR CURRENT|name":  set name of default subdirectory. */
        else if (lkchar("DIR#$", 6, MCPFN, kmdfm.krddir, MCPFN + 1, &nchar)) {
            if (memcmp(kmdfm.krddir, "CURRENT", 7) == 0 ||
                memcmp(kmdfm.krddir, "current", 7) == 0) {
                fstrncpy(kmdfm.krddir, MCPFN, " ", 1);
            } else if (kmdfm.krddir[nchar - 1] != KDIRDL) {
                dirDelimiter[0] = KDIRDL;
                dirDelimiter[1] = '\0';
                subscpy(kmdfm.krddir, nchar, -1, MCPFN, dirDelimiter);
            }
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

        /* -- "SCALE ON|OFF":  turn scaling on or off */
        else if (lklog("SCALE$", 7, &lscale)) {
            cmdfm.lscale = lscale;
        }

        else
            break;
    }                           /* end while */
    DEBUG("Parse Position independent tokens\n");
    /* - Parse position-independent tokens: */
    /* - Loop on each token in command: */
    while (lcmore(nerr)) {
        DEBUG("PITs\n");
        /* -- "filelist":  define a new input filelist. */
        if ((list = lcdfl())) { /* do nothing */
        }

        /* -- Bad syntax. */
        else {
            DEBUG("in PITs error: %d\n", *nerr);
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
        DEBUG("PITs: done\n");
    }
    DEBUG("nerr: %d\n", *nerr);
    if (*nerr != 0)
        goto L_8888;

    /* CHECKING PHASE: */

    if (!list) {
        list = string_list_init();
        string_list_extend(list, last_list);
    }
    /* ----- How many files will be in SAC memory?  Take no action if this
     *       read will exceed the max number of files that SAC can store. */

    if (lmore) {

        /* Commit or rollback existing data as per user specs. */
        alignFiles(nerr);
        if (*nerr)
            return;

        cmdfm.nfilesFirst = saclen();
    } /* end if( lmore ) */
    else {
        cmdfm.nreadflag = HIGH;
        cmdfm.nfilesFirst = 0;
    }

    /* EXECUTION PHASE */
    ldata = TRUE;
    lsdd = FALSE;
    DEBUG("readfl\n");
    /* --- Now go read the data files. */
    readfl(ldata, lmore, lsdd, kmdfm.krddir, MCPFN + 1, list, nerr);
    DEBUG("readfl: done\n");
    if (*nerr) {
        setmsg("ERROR", *nerr);
        goto L_8887;
    }

    /* put it out to SeisMgr */
    if (cmdfm.ltrust && cmdfm.nreadflag != LOW)
        cmdfm.nreadflag = HIGH;
    else
        cmdfm.nreadflag = LOW;

    cmdfm.lread = TRUE;
    sacToSeisMgr(!lmore, 0, 1, nerr);
    cmdfm.lread = FALSE;

  L_8887:
    /* Copy Current List to Last_List */
    string_list_clear(last_list);
    string_list_extend(last_list, list);
    string_list_free(list);
    list = NULL;

  L_8888:
    ftype = sacfi;
    return;

}
