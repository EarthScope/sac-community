/** 
 * @file   vfeven.c
 * 
 * @brief  Verify evenly spaced files available
 * 
 */

#include <string.h>

#include "amf.h"
#include "dfm.h"
#include "bool.h"
#include "hdr.h"

#include "errors.h"

#include "msg.h"
#include "clf.h"
#include "dff.h"

/** 
 * Verify that only evenly spaced time series files are in the data file list
 * 
 * @param nerr 
 *    - 0 on Success
 *    - ERROR_OPERATION_ON_UNEVEN_FILE
 *    - ERROR_OPERATION_ON_SPECTRAL_FILE
 *    - ERROR_OPERATION_ON_XYZ_FILE
 *
 * @date   850322:  Fixed logic in checking for spectral files.
 * @date   820622:  Original version.
 *
 */
void
vfeven(int *nerr) {

    int jdfl;
    char *tmp;
    *nerr = 0;
    sac *s;
    /* - For each file in DFL: */
    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        /* -- Get header from memory manager. */
        if (!(s = sacget(jdfl - 1, FALSE, nerr))) {
            goto L_8888;
        }
        tmp = s->m->filename;
        //getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );

        /* -- Check file type. */
        if (!s->h->leven) {
            *nerr = ERROR_OPERATION_ON_UNEVEN_FILE;
            setmsg("ERROR", *nerr);
            apcmsg2(tmp, strlen(tmp) + 1);
            goto L_8888;
        } else if (s->h->iftype == IRLIM || s->h->iftype == IAMPH) {
            *nerr = ERROR_OPERATION_ON_SPECTRAL_FILE;
            setmsg("ERROR", *nerr);
            apcmsg2(tmp, strlen(tmp) + 1);
            goto L_8888;
        } else if (s->h->iftype == IXYZ) {
            *nerr = ERROR_OPERATION_ON_XYZ_FILE;
            setmsg("ERROR", *nerr);
            goto L_8888;
        }
    }

  L_8888:
    return;
}
