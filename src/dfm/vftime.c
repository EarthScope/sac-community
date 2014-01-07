/** 
 * @file   vftime.c
 * 
 * @brief  Verify that only time series files are avaialbe
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
 * Verify that only time series files are in the data file list
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_OPERATION_ON_SPECTRAL_FILE
 *
 * @date   900503:  Added check for xyz data file type.
 * @date   820622:  Original version.
 *
 */
void 
vftime(int *nerr) {

	int jdfl;
    char *tmp;
    sac *s;
    *nerr = 0;

	/* - For each file in DFL: */
	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
    if(!(s = sacget(jdfl-1, FALSE, nerr))) {
      goto L_8888;
    }
    tmp = s->m->filename;
    /* -- Get header from memory manager. */
		//getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );

		/* -- Check file type. */
		if( s->h->iftype != ITIME && 
		    s->h->iftype != IXY   && 
		    s->h->iftype != IXYZ ){
			*nerr = ERROR_OPERATION_ON_SPECTRAL_FILE;
			setmsg( "ERROR", *nerr );
            apcmsg2(tmp, strlen(tmp)+1);
			goto L_8888;
		}
	}

L_8888:
	return;
}

