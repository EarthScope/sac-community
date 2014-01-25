/** 
 * @file   xsystemcomma.c
 * 
 * @brief  SYSTEMCOMMAND command
 * 
 */

#include "exm.h"
#include "co.h"
#include "cpf.h"

/** 
 * Execute the systemcommand which allows the user to execute a system command
 * 
 * @param nerr 
 *    Error Return Flag
 *
 * @date   870211:  Changed due to modification of LCREST.
 * @date   820817:  Changed to newest set of parsing and checking functions.
 * @date   820115:  Cleaned up and documented.
 *
 */
void
xsystemcommand(int *nerr) {

	char kmsg[MCMSG+1];
	int ncmsg;

	*nerr = 0;

	/* - Store rest of command line in output message buffer. */
	if( !lcrest( MCMSG, kmsg,MCMSG+1, &ncmsg ) ){
		cerr( 1001 );
		goto L_8888;
        }

	/* - Execute the system command. */
	zsysop( kmsg,MCMSG+1, &ncmsg, nerr );

L_8888:
	return;

}

