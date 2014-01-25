/** 
 * @file   xunsetbb.c
 * 
 * @brief  UNSETBB command
 * 
 */

#include <string.h>

#include "exm.h"
#include "bbs.h"
#include "cpf.h"
#include "msg.h"
#include "errors.h"

/** 
 * Execute the unsetbb command which deletes a blackboard variable
 * 
 * @param nerr 
 *    Error Return Flag
 *   - 0 on Success
 *
 * @date   880412:  Original version.
 *
 */
void 
xunsetbb(int *nerr) {

	char kname[MCMSG+1];
	int nchar;

	*nerr = 0;
    memset(kname, 0, sizeof(kname));
	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "ALL": unset all blackboard variables by deleting the current
		 *     blackboard and creating a new one. */
		if( lckey( "ALL#$",6 ) ){
			deletebbs( nerr );
			if( *nerr != 0 )
				goto L_8888;
			createbbs( nerr );
			if( *nerr != 0 )
				goto L_8888;

			/* -- "name":  the name of a specific blackboard variable to unset. */
			}
		else if( lcchar( MCMSG, kname,MCMSG+1, &nchar ) ){
			unsetbbv( kname, nerr, MCMSG );
			if( *nerr != 0 ) {
        if(*nerr == ERROR_DELETING_VARIABLE) {
          /* If variable does not exist, show the error and clear it immediately */
          outmsg();
          clrmsg();
        } else {
          goto L_8888;
        }
      }
			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}
L_8888:
	return;
}

