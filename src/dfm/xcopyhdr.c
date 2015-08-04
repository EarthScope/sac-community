/** 
 * @file   xcopyhdr.c
 * 
 * @brief  Copy header variables
 * 
 */

#include <string.h>

#include "dfm.h"
#include "bool.h"
#include "hdr.h"
#include "lhf.h"

#include "clf.h"
#include "cpf.h"
#include "dff.h"
#include "amf.h"
#include "errors.h"

DFM_EXTERN

int *const Icatco = &cmdfm.icatco[0] - 1;
int *const Itemco = &cmdfm.itemco[0] - 1;

/** 
 * Execute the command COPYHDR which copies header varibles from one
 *    file to the remaining file in the data file list
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   880413:  Was not reinitializing header list correctly.
 * @date   870209:  Converted to an internal command.
 * @date   840906:  Original XSC version.
 *
 */
void
xcopyhdr(int *nerr) {

    char ktemp1[MCPFN + 1], ktemp2[9], ktemp3[24];
    int lfirst, lfound, ltemp;
    int icatcox, idflco, itemcox, itemp;
    int jdfl, jhdrco, nhdrco, ntemp;
    float ftemp;
    sac *s;
    float *fp;
    int *ip;

    *nerr = 0;
    memset(ktemp1, 0, sizeof(ktemp1));
    memset(ktemp2, 0, sizeof(ktemp2));
    /* - Loop on each token in command: */
    lfirst = TRUE;
    idflco = 1;
    nhdrco = 0;
  L_1000:
    if (lcmore(nerr)) {

        /* -- "FROM name|n":  determine which file to copy from. */
        if (lckey("FROM#$", 7)) {
            if (lcirc(1, saclen(), &idflco)) {
            } else if (lcchar(ktemp1, sizeof(ktemp1))) {
                char *ktemp2 = fstrdup(ktemp1, MCPFN + 1);
                idflco = sac_find_filename(ktemp2);
                if (idflco < 0) {
                    arg_prev();
                    cfmt("BAD FILE NAME:", 16);
                    cresp();
                }
                idflco += 1;
            } else {
                cfmt("NEED A FILE NAME OR A NUMBER:", 31);
                cresp();
            }
        }
        /* -- "hdrvar":  name of a header variable to copy. */
        else if (lcchar(ktemp2, sizeof(ktemp2))) {
            hdrfld(ktemp2, 9, &icatcox, &itemcox, &lfound);
            if (lfound) {
                if (lfirst) {
                    nhdrco = 0;
                    lfirst = FALSE;
                }
                if (nhdrco < MHDRCO) {
                    nhdrco = nhdrco + 1;
                    Icatco[nhdrco] = icatcox;
                    Itemco[nhdrco] = itemcox;
                } else {
                    cfmt("TOO MANY HEADER VARIABLES:", 28);
                    cresp();
                }
            } else {
                cfmt("ILLEGAL HEADER VARIABLE:", 26);
                cresp();
            }

            /* -- Bad syntax. */
        } else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();

        }
        goto L_1000;

    }

    if (*nerr != 0)
        goto L_8888;

    /* CHECKING PHASE: */
    /* - Test for a non-null data file list. */

    vflist(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* EXECUTION PHASE: */
    /* - Loop on each header itemco in list. */

    for (jhdrco = 1; jhdrco <= nhdrco; jhdrco++) {
        ftemp = SAC_FLOAT_UNDEFINED;
        itemp = SAC_INT_UNDEFINED;
        ntemp = SAC_ENUM_UNDEFINED;
        ltemp = SAC_LOGICAL_UNDEFINED;
        strncpy(ktemp3, SAC_CHAR_UNDEFINED, 8);
        strncpy(ktemp3, SAC_CHAR_UNDEFINED, 8);
        /* -- Get master file's header from memory manager. */
        if (!(s = sacget(idflco - 1, TRUE, nerr))) {
            //getfil( idflco, FALSE, &notusd, &notusd, &notusd, nerr );
            *nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
            goto L_8888;
        }
        /* -- Get header variable from master file. */
        if (Icatco[jhdrco] == FLOAT_TYPE) {
            ftemp = VALUE(fhdr(s, Itemco[jhdrco]));
        } else if (Icatco[jhdrco] == INT_TYPE) {
            itemp = VALUE(ihdr(s, Itemco[jhdrco]));
        } else if (Icatco[jhdrco] == ENUM_TYPE) {
            ntemp = VALUE(nhdr(s, Itemco[jhdrco]));
        } else if (Icatco[jhdrco] == LOGICAL_TYPE) {
            ltemp = VALUE(lhdr(s, Itemco[jhdrco]));
        } else if (Icatco[jhdrco] == STRING_TYPE) {
            strcpy(ktemp3, khdr(s, Itemco[jhdrco]));
        }

        /* -- Copy this variable to all (other) files in DFL. */
        for (jdfl = 1; jdfl <= saclen(); jdfl++) {
            if (!(s = sacget(jdfl - 1, FALSE, nerr))) {
                goto L_8888;
            }
            //getfil( jdfl, FALSE, &notusd, &notusd, &notusd, nerr );

            if (Icatco[jhdrco] == FLOAT_TYPE) {
                fp = fhdr(s, Itemco[jhdrco]);
                VALUE(fp) = ftemp;
            } else if (Icatco[jhdrco] == INT_TYPE) {
                ip = ihdr(s, Itemco[jhdrco]);
                VALUE(ip) = itemp;
            } else if (Icatco[jhdrco] == ENUM_TYPE) {
                ip = nhdr(s, Itemco[jhdrco]);
                VALUE(ip) = ntemp;
            } else if (Icatco[jhdrco] == LOGICAL_TYPE) {
                ip = lhdr(s, Itemco[jhdrco]);
                VALUE(ip) = ltemp;
            } else if (Icatco[jhdrco] == STRING_TYPE) {
                strcpy(khdr(s, Itemco[jhdrco]), ktemp3);
            }
        }

    }
  L_8888:
    return;
}
