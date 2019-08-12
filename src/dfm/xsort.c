/** 
 * @file xsort.c
 *
 * @brief Sort data file list based on a header value
 */

#include <stdlib.h>
#include <string.h>

#include "dfm.h"
#include "lhf.h"
#include "hdr.h"

#include "cssListOps/cssStrucs.h"
#include "cssListOps/cssListStrucs.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "smDataIO.h"
#include "sm_gc.h"

#include "msg.h"
#include "bot.h"
#include "ssi.h"
#include "cpf.h"
#include "dff.h"
#include "amf.h"

DFM_EXTERN
LHF_EXTERN

static int nSortOrder;

static int
compare(const void *pa, const void *pb) {
    int i, icat, item, lok;
    SortOrder dir;
    double fa, fb;
    int na, nb;
    char ca[64], cb[64];
    sac *a = *((sac **) pa);
    sac *b = *((sac **) pb);
    for (i = 0; i < nSortOrder; i++) {
        dir = cmdfm.idirection[i];
        hdrfld(kmdfm.ksort[i], 9, &icat, &item, &lok);
        if (!lok || icat > STRING_TYPE) {
            error(1381, " %s", kmdfm.ksort[i]);
            outmsg();
            continue;
        }
        switch (icat) {
            case FLOAT_TYPE:
                sac_get_float(a, item, &fa);
                sac_get_float(b, item, &fb);
                if (fa == fb) {
                    continue;
                }
                switch (dir) {
                    case Ascending:
                        return ((fa > fb) ? 1 : -1);
                        break;
                    case Descending:
                        return ((fa > fb) ? -1 : 1);
                        break;
                }
                break;
            case INT_TYPE:
            case LOGICAL_TYPE:
                na = (icat == INT_TYPE) ? NHDR(a)[item] : LHDR(a)[item];
                nb = (icat == INT_TYPE) ? NHDR(b)[item] : LHDR(b)[item];
                if (na == nb) {
                    continue;
                }
                switch (dir) {
                    case Ascending:
                        return (na > nb) ? 1 : -1;
                        break;
                    case Descending:
                        return (na < nb) ? -1 : 1;
                        break;
                }
                break;
            case ENUM_TYPE:
            case STRING_TYPE:
                if (icat == ENUM_TYPE) {
                    strcpy(kmlhf.kiv[IHDR(a)[item] - 1], ca);
                    strcpy(kmlhf.kiv[IHDR(b)[item] - 1], cb);
                } else {
                    strcpy(&KHDR(a)[item], ca);
                    strcpy(&KHDR(b)[item], cb);
                }
                if (strcmp(ca, cb) != 0) {
                    switch (dir) {
                        case Ascending:
                            return strcmp(ca, cb);
                            break;
                        case Descending:
                            return strcmp(cb, ca);
                            break;
                    }
                }
        }
    }
    return 0;
}

/** 
 * Sort the data file list (DFL) based on a header value.
 *
 * @param *nerr
 *    Error return flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @return Nothing
 *
 * @note ASCEND Sort ascendingly
 * @note COMMIT RECALLTRACE RECALL ROLLBACK Database functionality
 * @note DESCEND Sort decendingly
 * @note Header field to sort on
 *
 * First, gather the header variable used to sort.  Sort indicies based 
 *  on the gathered header variables using Index(). Finally, reorder the
 *  variables which define the order of the sac files in memory.
 *  The routine dblSortWfdiscList was doing the reordering, but did not
 *  work as of 070606 and was then wrapped in an if-else block.
 *
 * @note realIdx - Value of current, before sorted, file ids
 * @note currIdx - Index values after sorting
 * @note tempIdx - Temporary index values for rearrangement
 *
 * @see Index
 *
 * @date 070606:  Documented/Reviewed
 * @date 070606:  if-else block now wraps the SeisMgr database 
 *                  and the basic reorder() routines.
 *
 */
void
xsort(int *nerr) {

    char ktemp[9];

    ktemp[0] = '\0';
    nSortOrder = 0;
    /* PARSING MODE */
    while (lcmore(nerr)) {

        /* -- "COMMIT|RECALLTRACE|ROLLBACK": how to treat existing data */
        if (lckeyExact("COMMIT", 7))
            cmdfm.icomORroll = COMMIT;
        else if (lckeyExact("RECALLTRACE", 12))
            cmdfm.icomORroll = RECALL;
        else if (lckeyExact("RECALL", 7))
            cmdfm.icomORroll = RECALL;
        else if (lckeyExact("ROLLBACK", 9))
            cmdfm.icomORroll = ROLLBACK;

        if (!lcmore(nerr))
            break;

        /* -- ASCEND */
        if (lckey("ASCEND#$", 9)) {
            if (nSortOrder > 0)
                cmdfm.idirection[nSortOrder - 1] = Ascending;
            else {
                setmsg("WARNING", 1384);
                outmsg();
                clrmsg();
            }
        }

        /* -- DESCEND */
        else if (lckey("DESCEND#$", 10)) {
            if (nSortOrder > 0)
                cmdfm.idirection[nSortOrder - 1] = Descending;
            else {
                setmsg("WARNING", 1384);
                outmsg();
                clrmsg();
            }
        }

        /* -- header names */
        else if (lcchar(ktemp, sizeof(ktemp))) {
            if (nSortOrder < MAXSORTFIELDS) {
                strcpy(kmdfm.ksort[nSortOrder], ktemp);
                cmdfm.idirection[nSortOrder] = Ascending;
                nSortOrder++;
            }
        }

        /* Bad syntax */
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }                           /* end while */

    /* if nSortOrder is > 0, then sort options were entered.  
       Otherwise, use sort options from the previous sort command. */
    if (nSortOrder)
        cmdfm.nSortOrder = nSortOrder;

    /* if no sort options have ever been entered, error out */
    if (cmdfm.nSortOrder == 0) {
        *nerr = 1379;
        setmsg("ERROR", *nerr);
        return;
    }

    sacsort(compare);

}                               /* end xsort() */
