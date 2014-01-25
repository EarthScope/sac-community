/** 
 * @file   xsyntx.c
 * 
 * @brief  SYNTAX command
 * 
 */

#include "exm.h"
#include "cpf.h"
#include "bool.h"


#include "ucf.h"

/** 
 * Execute the SYNTAX command which prints syntax info from the help system
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   831013:  Original version (from XHELP.)
 *
 */
void 
xsyntx(int *nerr) {

	char ktoken[9];
	int lnumbr;

	*nerr = 0;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "token":  the name of a help package. */
		if( lcchar( 9,ktoken,9, &lnumbr) ){
			wrhelp( ktoken,9, 2, FALSE, nerr );
    }
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}
	return;
}

