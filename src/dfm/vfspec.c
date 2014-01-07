/** 
 * @file   vfspec.c
 * 
 * @brief  Verify that only spectral files are available
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
 * Verify that only spectral files are in the data file list
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_OPERATION_ON_TIME_SERIES_FILE
 *
 * @date   820622:  Original version.
 *
 */
void 
vfspec(int *nerr) {

	int jdfl;
    char *tmp;
    sac *s;
    *nerr = 0;

	/* - For each file in DFL: */
	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
		/* -- Get header from memory manager. */
    if(!(s = sacget(jdfl-1, FALSE, nerr))) {
      goto L_8888;
    }
    tmp = s->m->filename;
		//getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );

		/* -- Check file type. */
		if( s->h->iftype != IRLIM && s->h->iftype != IAMPH ){
			*nerr = ERROR_OPERATION_ON_TIME_SERIES_FILE;
			setmsg( "ERROR", *nerr );
            apcmsg2(tmp, strlen(tmp)+1);
			goto L_8888;
		}
	}

L_8888:
	return;
}

