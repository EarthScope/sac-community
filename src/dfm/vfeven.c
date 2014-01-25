/** 
 * @file   vfeven.c
 * 
 * @brief  Verify evenly spaced files available
 * 
 */

#include <string.h>

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

	int jdfl, ndx1, ndx2, nlen;
    char *tmp;
	*nerr = 0;

	/* - For each file in DFL: */
	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
        tmp = string_list_get(datafiles, jdfl-1);
		/* -- Get header from memory manager. */
		getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Check file type. */
		if( !*leven ){
			*nerr = ERROR_OPERATION_ON_UNEVEN_FILE;
			setmsg( "ERROR", *nerr );
            apcmsg2(tmp, strlen(tmp)+1);
			goto L_8888;
		}
		else if( *iftype == *irlim || *iftype == *iamph ){
			*nerr = ERROR_OPERATION_ON_SPECTRAL_FILE;
			setmsg( "ERROR", *nerr );
            apcmsg2(tmp, strlen(tmp)+1);
			goto L_8888;
		}
                else if( *iftype == *ixyz ){
                        *nerr = ERROR_OPERATION_ON_XYZ_FILE;
			setmsg( "ERROR", *nerr );
                        goto L_8888;
		}
	}

L_8888:
	return;
}

