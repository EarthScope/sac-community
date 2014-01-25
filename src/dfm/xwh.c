/** 
 * @file   xwh.c
 * 
 * @brief  Write a SAC Header
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dfm.h"
#include "bool.h"
#include "hdr.h"



#include "msg.h"
#include "clf.h"
#include "ssi.h"
#include "cpf.h"
#include "dff.h"

/** 
 * Execute the command WRITEHDR which writes a SAC Header
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   850801:  Deleted SOCKITOME format.
 * @date   830930:  Added error condition when CUT option is on.
 * @date   820721:  Changed to newest set of parsing and checking functions.
 * @date   801003:  Original version.
 * @date   810223:  Added check for null data file list.
 *
 */
void 
xwh(int *nerr) {

  char *tmp;
	int jdfl, ndx1, ndx2, nlen;

	*nerr = 0;

	if ( lcmore ( nerr ) ) {
	    /* -- "COMMIT|RECALLTRACE|ROLLBACK": how to treat existing data */
	    if ( lckeyExact ( "COMMIT" , 7 ) )
		cmdfm.icomORroll = COMMIT ;
	    else if (lckeyExact ( "RECALLTRACE" , 12 ) )
		cmdfm.icomORroll = RECALL ;
	    else if ( lckeyExact ( "RECALL" , 7 ) )
		cmdfm.icomORroll = RECALL ;
	    else if ( lckeyExact ( "ROLLBACK" , 9 ) )
		cmdfm.icomORroll = ROLLBACK ;
	}


	/* CHECKING PHASE: */
	/* - Check for null data file list. */
	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Make sure CUT is off. */
	if( cmdfm.lcut ){
		*nerr = 1341;
		setmsg( "ERROR", *nerr );
		goto L_8888;
	}

	/* EXECUTION PHASE: */
        alignFiles ( nerr ) ;
	if ( *nerr )
	    return ;


	/* - For each file in DFL: */
	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
	  /* -- Get header from working memory or disk determine file name. */
		getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );
		if( *nerr != 0 )
			goto L_8888;
        tmp = string_list_get(datafiles, jdfl-1);
		/* -- Check to see if overwrite flag is enabled. */
		if( !*lovrok ){
			*nerr = 1303;
			setmsg( "ERROR", *nerr );
            apcmsg2(tmp, strlen(tmp)+1);
			goto L_8888;
		}

		/* -- Write header. */
		wrsac( jdfl, tmp, strlen(tmp)+1, FALSE, nerr );

		if( *nerr != 0 )
			goto L_8888;

	}

L_8888:
	return;
}

