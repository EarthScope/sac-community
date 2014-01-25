
#include <string.h>

#include "xyz.h"
#include "dfm.h"
#include "hdr.h"
#include "bool.h"


#include "msg.h"
#include "clf.h"
#include "dff.h"

void /*FUNCTION*/ vfxyz(nerr)
int *nerr;
{
	int jdfl, ndx1, ndx2, nlen;
  char *tmp;

	/*=====================================================================
	 * PURPOSE:  To verify that only xyz data files are in data file list.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error return flag.  Set to zero if no error occurs.
	 *             Set to 1366 if all files are not xyz data files.
	 *=====================================================================
	 * MODULE/LEVEL:  xyz/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    dfm:     ndfl, kdfl
	 *    hdr:     iftype, ixyz
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:     getfil, setmsg, lnumcl, apcmsg
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900305:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900305
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - For each file in DFL: */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
    tmp = string_list_get(datafiles, jdfl-1);
		/* -- Get header from memory manager. */
		getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Check file type. */
		if( *iftype != *ixyz ){
			*nerr = 1366;
			setmsg( "ERROR", *nerr );
            apcmsg(tmp, strlen(tmp)+1);
			goto L_8888;
			}

		}


L_8888:
	return;

} /* end of function */

