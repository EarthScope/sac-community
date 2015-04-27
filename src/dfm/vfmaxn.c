/** 
 * @file   vfmaxn.c
 * 
 * @brief  Verify that no files have more than npts
 * 
 */

#include <string.h>

#include "dfm.h"
#include "hdr.h"
#include "bool.h"

#include "errors.h"

#include "amf.h"
#include "msg.h"
#include "clf.h"
#include "dff.h"

/** 
 * Verify that no files in the data file list have more than
 *    a certain number of data points
 * 
 * @param maxn 
 *    Maximum number of data points that can be in each file
 * @param maxf 
 *    Maximum number of points found in the files
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_TOO_MANY_DATA_POINTS
 *    
 * @date   820622:  Original version.
 *
 */
void
vfmaxn(int maxn, int *maxf, int *nerr) {

    int jdfl;
    char *tmp;
    sac *s;
    *nerr = 0;
    *maxf = 0;

    /* - For each file in DFL: */
    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        if (!(s = sacget(jdfl - 1, FALSE, nerr))) {
            goto L_8888;
        }
        tmp = s->m->filename;
        /* -- Get header from memory manager. */
        //getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );

        /* -- See if this files number exceeds maximum found so far. */
        if (s->h->npts > *maxf)
            *maxf = s->h->npts;

        /* -- Check number of data points versus maximum allowable. */
        if (s->h->npts > maxn) {
            *nerr = ERROR_TOO_MANY_DATA_POINTS;
            setmsg("ERROR", *nerr);
            apimsg(maxn);
            apcmsg2(tmp, strlen(tmp) + 1);
            goto L_8888;
        }
    }

  L_8888:
    return;
}
