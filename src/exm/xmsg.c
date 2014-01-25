/** 
 * @file   xmsg.c
 * 
 * @brief  MESSAGE command
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "exm.h"
#include "cpf.h"
#include "msg.h"

/** 
 * Execute the MESSAGE command which send a message to the user by carrier pigeon
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   820809:  Changed to newest set of parsing and checking functions.
 * @date   820115:  Cleaned up logic and documented subroutine.
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   800605:  Original version.
 *
 */
void 
xmsg(int *nerr) {

	char kmsg[MCMSG+1];
	int ncmsg;

	*nerr = 0;

    memset(kmsg, 0, MCMSG+1);
	/* PARSING PHASE: */

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "message":  message to write to the user's terminal. */
		if( lcchar_split( MCMSG, kmsg,MCMSG+1, &ncmsg ) ){
			setmsg( "OUTPUT", 99 );
			apcmsg( kmsg,MCMSG+1 );
			outmsg();
			clrmsg();

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}
	return;
}

