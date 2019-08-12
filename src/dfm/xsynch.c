/** 
 * @file   xsynch.c
 * 
 * @brief  Sort 
 * 
 */

#include <fern/array.h>

#include "dfm.h"
#include "bool.h"
#include "hdr.h"

#include "co.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"
#include "amf.h"
#include "errors.h"

DFM_EXTERN

/** 
 * Execute the command SORT to sort the data file list
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   831020:  Moved from XSC to internal DFM command.
 * @date   830915:  Merged SYNCH and SYNCH2 using new shell.
 * @date   830124:  Modifications due to new set of parsing/checking functions.
 * @date   810624:  Original version of XSC shell.
 *
 */
void
xsynch(int *nerr) {

    int i, nb, *ndttmi, *ndttmo;
    double *begi, *bego, dtnew;

    static int lbegin = FALSE;
    sac *s;
    *nerr = 0;
    begi = xarray_new_with_len('d', saclen());
    bego = xarray_new_with_len('d', saclen());
    ndttmi = xarray_new_with_len('i', saclen() * 6);
    ndttmo = xarray_new_with_len('i', saclen() * 6);

    /* PARSING PHASE */
    /* - Parse each token in command (if any): */
    while (lcmore(nerr)) {

        /* -- "ROUND [ON/OFF]":  change begin time rounding option. */
        if (lklog("R#OUND$", 8, &cmdfm.lround)) {    /* do nothing */
        }

        /* -- "BEGIN [ON/OFF]":  set both begin times to zero if ON. */
        else if (lklog("B#EGIN$", 8, &lbegin)) {        /* do nothing */
        }

        else {
            /* -- Bad syntax. */
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }

    if (*nerr != 0)
        return;

    /* CHECKING PHASE: */
    /* - Test for a non-null data file list. */
    vflist(nerr);
    if (*nerr != 0)
        return;

    /* - Make sure each file is an evenly spaced time series file. */
    vfeven(nerr);
    if (*nerr != 0)
        return;

    /* EXECUTION PHASE: */
    /* - Save beginning offset and reference time for each file. */
    for (i = 0; i < saclen(); i++) {
        if (!(s = sacget(i, TRUE, nerr))) {
            *nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
            return;
        }
        //getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );

        begi[i] = B(s);
        copyi(&s->h->nzyear, &ndttmi[i * 6], 6);
    }

    /* - Calculate new reference times and beginning offsets. */
    synch((int (*)[6]) ndttmi, begi, saclen(), (int (*)[6]) ndttmo, bego,
          lbegin);

    /* - For each file in DFL: */
    for (i = 0; i < saclen(); i++) {
        if (!(s = sacget(i, TRUE, nerr))) {
            return;
        }
        /* -- Get next file from the memory manager.
         *    (Header is moved into common blocks CMHDR and KMHDR.) */
        //getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

        /* -- Update time header fields.
         *    New beginning offsets are set to exact value or they
         *    may be rounded to the nearest multiple of DELTA. */
        dtnew = bego[i] - B(s);
        if (cmdfm.lround) {
            nb = (int) (bego[i] / DT(s) + sign(0.5, B(s)));
            sac_set_float(s, SAC_B, DT(s) + (double) nb);
        } else {
            sac_set_float(s, SAC_B, B(s) + dtnew);
        }
        sac_be(s);
        for(int j = SAC_O; j <= SAC_F; j++) {
            double v = 0.0;
            sac_get_float(s, j, &v);
            if (v != SAC_FLOAT_UNDEFINED) {
                v = v + dtnew;
                sac_set_float(s, j, v);
            }
        }
        copyi(&ndttmo[i * 6], &s->h->nzyear, 6);

    }
    xarray_free(begi);
    xarray_free(bego);
    xarray_free(ndttmi);
    xarray_free(ndttmo);
    return;
}
