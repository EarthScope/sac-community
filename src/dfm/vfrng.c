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
vfrng(double  rngmin, 
      double  rngmax, 
      int    *nerr) {

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

		/* -- Check range of dependent variable. */
		if( *depmin < rngmin || *depmax > rngmax ){
			*nerr = ERROR_DATA_POINTS_OUTSIDE_OF_RANGE;
			setmsg( "ERROR", *nerr );
			apfmsg( rngmin );
			apfmsg( rngmax );
            apcmsg2(tmp, strlen(tmp)+1);
			goto L_8888;
		}
	}
L_8888:
	return;
}

