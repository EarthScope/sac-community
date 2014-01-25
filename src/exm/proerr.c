/** 
 * @file   proerr.c
 * 
 * @brief  Process an error
 * 
 */
#include <string.h>

#include "exm.h"

#include "msg.h"
#include "co.h"
#include "cpf.h"
#include "ncpf.h"

void 
proerr(int *nerr) {

	/*=====================================================================
	 * PURPOSE: To process a detected error.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    nerr:    Error flag.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Always set to zero upon return.
	 *=====================================================================
	 * MODULE/LEVEL:  exm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    com:     lcomf
	 *    exm:     lprod, kecnof
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  wrtxtt, wrcom, zquit, cszero, outmsg, clrmsg
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Certain error conditions can be controlled by user.
	 *   Handle these here. */
	/* -- Error condition when there are no data files in memory. */
	if( *nerr == 1301 ){
		if( strcmp(kmexm.kecnof,"WARNING ") == 0 ){
			typmsg( "WARNING" );
			outmsg();
			clrmsg();
			*nerr = 0;
			goto L_8888;
			}
		else if( strcmp(kmexm.kecnof,"IGNORE  ") == 0 ){
			*nerr = 0;
			clrmsg();
			goto L_8888;
			}
		}

	/* - Write the error message and current command to error devices.
	 *   (Skip this step if this was a command syntax error.) */
	if( *nerr != 1001 )
		outmsg();
	/* - Quit if this is a production run. */

	if( cmexm.lprod ){
		setmsg( "ERROR", 1101 );
		outmsg();
		zquit();
		}

	/* - Inform macro system that an error has occurred. */

	setmacrostatus( "ERROR",6 );

	/* - Warn user if this command was from a command file. */

	if( cmexm.lcomf ){
		setmsg( "WARNING", 1102 );
		outmsg();
		}

	/* - Clear error condition. */

	clrmsg();
	*nerr = 0;

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    870722:  Added call to setmacrostate.
	 *    860130:  Deleted old message logic.
	 *    841005:  Added logic to distiguish old/new message handling logic.
	 *    820909:  Put user controllable error handling into this routine.
	 *    810120:  Changed to output message retrieval from disk.
	 *===================================================================== */

} /* end of function */

