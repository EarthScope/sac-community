/** 
 * @file   xinstallmacr.c
 * 
 * @brief  INSTALLMACRO command
 * 
 */

#include "exm.h"
#include "cpf.h"
#include "msg.h"
#include "bool.h"


#include "co.h"
#include "bot.h"

/** 
 * Execute the INSTALLMACRO command which installs a SAC macro 
 *    in the global directory
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   920305:  Changed the DO loop to allow installation of more than
 *             one macro at the time as described in the manual.
 *             Guy Tytgat.
 * @date   871222:  Moved file inquire to zinquire.
 * @date   870921:  Moved most of procedure for copying to zauxfile.
 * @date   870416:  Original version.
 *
 */
void 
xinstallmacro(int *nerr) {

	char kname[MCPFN+1];
	int lexist;
	int nchar;

	*nerr = 0;


	while( lcmore( nerr ) ){

		/* -- "text":  the name of a macro to install. */
		if( lcchar( MCPFN, kname,MCPFN+1, &nchar ) ){
			modcase( FALSE, kname, nchar, kname );

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}


		/* - Make sure macro file exists. */

		zinquire( kname, &lexist );

		if( !lexist ){
			*nerr = 108;
			setmsg( "ERROR", *nerr );
			apcmsg( kname,MCPFN+1 );
			goto L_8888;
			}

		/* - Install macro in the SAC auxiliary directory. */

		zauxfile( "macros",7, kname,MCPFN+1, nerr );

		}

L_8888:
	return;
}

