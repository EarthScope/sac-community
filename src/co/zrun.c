/** 
 * @file   zrun.c
 * 
 * @brief  Execute a runfile
 * 
 */

#include <stdio.h>
#include <string.h>

#include "co.h"
#include "bot.h"

/** 
 * Execute a runfle produced by zrunname and zruntext
 * 
 * @param nfun 
 *    File descriptor
 * @param runfile 
 *    Name of run file 
 * @param runfile_s 
 *    Length of \p runfile
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   871014:  Original version.
 *
 * @bug Only used by macro subsystem and only when the RUN command is specified
 *         This behavior is taken care of using the underlying system.
 *
 */
void 
zrun(FILE **nfun, 
     char  *runfile, 
     int    runfile_s, 
     int   *nerr) {

	char runtext[81];
	int nc;

    memset(runtext, 0, sizeof(runtext));
	/* - Write last line to the runfile. */
        fprintf(*nfun,"%s\n","endrun");

	/* - Close runfile. */
	zcloses( nfun, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Execute the runfile as a shell script using "zsysop". */
	subscpy( runtext, 0, 2, 80, "sh " );
	subscpy( runtext, 3, -1, 80, runfile );
	nc = indexb( runtext,81 );
	zsysop( runtext,81, &nc, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Destroy the runfile. */
	zdest( runfile,runfile_s, nerr );

L_8888:
	return;

}

