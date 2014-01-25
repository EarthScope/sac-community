/** 
 * @file executemacro.c
 *
 * @brief Execute a SAC macro
 *
 */
#include <string.h>

#include "ncpf.h"


#include "msg.h"
#include "top.h"

#define	MCMACROLINE	1000

/** 
 * Execute a SAC macro command file
 *
 * @param kmacroname
 *    Name of SAC Macro
 * @param kmacroname_s
 *    Length of \p kmacroname
 * @param kmacroargs
 *    Text of arguments to macro
 * @param kmacroargs_s
 *    Length of \p kmacroargs
 * @param nerr
 *    Error return flag
 *    - 0 on Success
 *    - Non-Zero on Failure
 *
 * @date   900129:  Changed arguments to openmacro, macroline, and closemacro.
 * @date   871109:  Increased size of "kline".
 * @date   870721:  Added storage of macro names by level number.
 *                  Added termination of macro logic upon execution error.
 * @date   870416:  Original version.
 *
 * @date   870416: Documented/Reviewed
 *
 */
void 
executemacro(char *kmacroname,
             int   kmacroname_s,
             char *kmacroargs,
             int   kmacroargs_s,
             int  *nerr) {

	char kmacroline[1001];
	int ncerr, ncmacroline;
	static int imacrolevel = 0;

	*nerr = 0;

	memset(&(kmacroline[0]), ' ', 1000);
	kmacroline[ 1000 ] = '\0' ;

	/* - Increment and set the macro nesting level counter.
	 *   Initialize status indicator. */

	imacrolevel = imacrolevel + 1;
	setmacrostatus( "OK",3 );
	setmacrolevel( imacrolevel );

	/* - Open macro file. */
	openmacro( kmacroname,kmacroname_s, kmacroargs,kmacroargs_s, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Get each line from macro file and execute it. */

L_1000:
	if( macroline( kmacroline,1000, &ncmacroline, nerr ) ){
		if( *nerr != 0 )
			goto L_7000;
		if( ncmacroline > 0 )
			saccommands( kmacroline,1000, nerr );
		if( *nerr != 0 || !macrostatus() ){
			*nerr = 1016;
			setmsg( "ERROR", *nerr );
			apcmsg( kmacroname,kmacroname_s );
			aplmsg( "Command line is:",17 );
                        apcmsg2(kmacroline,ncmacroline);
			goto L_7000;
			}
		goto L_1000;
		}

	/* - Close macro file and decrement macro level counter. */

L_7000:
	closemacro( &ncerr );
	imacrolevel = imacrolevel - 1;
	setmacrolevel( imacrolevel );

L_8888:
	return;

} /* end of function */

