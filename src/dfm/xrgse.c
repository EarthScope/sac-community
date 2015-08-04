/** 
 * @file   xrgse.c
 * 
 * @brief  Read a GSE File
 * 
 */
#include <string.h>

#include "amf.h"
#include "dfm.h"
#include "com.h"
#include "bool.h"

#include "co.h"
#include "ssi.h"
#include "cpf.h"
#include "clf.h"

DFM_EXTERN

int magnitude_option();

/** 
 * Execute the command READGSE which reads in a GSE File
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   970206:  New option to specify which magnitude to read.  maf
 * @date   910402:  New code.
 *
 */
void
xrgse(int *nerr) {

    int nchar;
    char _c0[2];
    int lmore, lshift, lscale;
    static int Verbose = 0;
    static string_list *last_list = NULL;
    char file[MCMSG + 1];

    int ret;
    string_list *list;
    double tmp;

    if (!last_list) {
        last_list = string_list_init();
    }
    *nerr = 0;
    list = string_list_init();
    /* PARSING PHASE: */
    /* - Parse position-dependent tokens: */
    lmore = FALSE;

    while (lcmore(nerr)) {

        /* -- "MORE":  signifies addition of more files to current 
         *             read filelist rather than replacement of 
         *             current list with new one. 
         */
        if (lckey("MORE#$", 7) && saclen() > 0) {
            lmore = TRUE;
            continue;
        }

        /* -- "VERBOSE ON|OFF":  turn Verbose mode on or off. */
        else if (lklog("VER$BOSE", 9, &Verbose)) {
            continue;
        }
        /* -- "SHIFT ON|OFF":  turn calibration on or off. */
        else if (lklog("SHIFT$", 7, &lshift)) {
            cmdfm.lshift = lshift;
            continue;
        }

        /* -- "SCALE ON|OFF":  turn time shift on or off. */
        else if (lklog("SCALE$", 7, &lscale)) {
            cmdfm.lscale = lscale;
            continue;
        }

        /* -- "MAXMEM v":  change maximum fractional memory used 
         *                 by SeisMgr. */
        else if (lkreal("MAX#MEM$", 9, &tmp)) {
            MaxMem = (float) tmp;
        }

        /* -- "DIR CURRENT|name":  set the name of the default 
         *                         subdirectory. */
        else if (lkchar("DIR#$", 6, MCPFN, kmdfm.krdcssdir, MCPFN + 1, &nchar)) {
            if (memcmp(kmdfm.krdcssdir, "CURRENT", 7) == 0 ||
                memcmp(kmdfm.krdcssdir, "current", 7) == 0) {
                cfmt("ILLEGAL PARAM VALUE: current", 30);
                cresp();
                return;
            } else if (kmdfm.krdcssdir[nchar - 1] != KDIRDL) {
                _c0[0] = KDIRDL;
                _c0[1] = '\0';
                subscpy(kmdfm.krdcssdir, nchar, -1, MCPFN, _c0);
            }
            continue;
        }

        /* -- "MAGNITUDE|mb|ms|ml|def":  specify a field for magnitude,
         *                               or if def is found, use the 
         *                               algorithm to determine which 
         *                               magnitude to read.
         */
        else if ((ret = magnitude_option())) {
            if (ret < 0) {
                return;
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

        /* -- Else assume it to be beginning/continuing dfl, if 
         *       not recognized as above Use kdflin as dummy 
         *       buffer to skip a string  
         */
        else if (lcchar(file, sizeof(file))) {
            string_list_put(list, file, -1);
        }
    }

    if (*nerr != 0)
        return;

    if (string_list_length(list) <= 0) {
        string_list_extend(list, last_list);
    }

    /* EXECUTION PHASE: */
    /* - Commit or rollback data according to lmore and cmdfm.icomORroll */
    if (lmore) {
        alignFiles(nerr);
        if (*nerr)
            return;
        cmdfm.nfilesFirst = saclen();
    } else
        cmdfm.nfilesFirst = 0;

    if (!list) {
        list = string_list_init();
    }

    string_list_clear(last_list);
    string_list_extend(last_list, list);

    /* - Expand the filelist and read the files into memory. */
    cmdfm.nreadflag = LOW;
    readgse(lmore, kmdfm.krdcssdir, MCPFN + 1, list, Verbose, cmdfm.lrascii,
            MaxMem, nerr);

    string_list_free(list);
    list = NULL;

    return;
}
