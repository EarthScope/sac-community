/** 
 * @file   vfrng.c
 * 
 * @brief  Verify that the dependent variables are within a range
 * 
 */

#include <string.h>

#include "dfm.h"
#include "bool.h"
#include "hdr.h"

#include "errors.h"

#include "amf.h"
#include "msg.h"
#include "clf.h"
#include "dff.h"

/** 
 * Verify that the dependent variable for all files in the data file list
 *    are in the allowed range
 * 
 * @param rngmin 
 *    Minimum allowed value
 * @param rngmax 
 *    Maximum allowed value
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_DATA_POINTS_OUTSIDE_OF_RANGE
 *
 * @date   820818:  Original version.
 *
 */
void
vfrng(double rngmin, double rngmax, int *nerr) {

    int jdfl;
    char *tmp;
    sac *s;
    *nerr = 0;

    /* - For each file in DFL: */
    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        if (!(s = sacget(jdfl - 1, FALSE, nerr))) {
            goto L_8888;
        }
        tmp = s->m->filename;
        /* -- Get header from memory manager. */
        //getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );

        /* -- Check range of dependent variable. */
        if (s->h->depmin < rngmin || s->h->depmax > rngmax) {
            *nerr = ERROR_DATA_POINTS_OUTSIDE_OF_RANGE;
            setmsg("ERROR", *nerr);
            apfmsg(rngmin);
            apfmsg(rngmax);
            apcmsg2(tmp, strlen(tmp) + 1);
            goto L_8888;
        }
    }
  L_8888:
    return;
}
