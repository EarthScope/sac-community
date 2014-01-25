/** 
 * @file   xcd.c
 * 
 * @brief  Change directories
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "exm.h"
#include "cpf.h"

/** 
 * Change SAC's current working directory
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   102795:  Original version.
 *
 */
void 
xcd(int *nerr) {
    char kname[MCPFN+1];
	int nc;

	*nerr = 0;
    memset(kname, 0, MCPFN+1);

L_1000:
	if( lcmore( nerr ) ){

         /* Get the name of a directory to change to. */
		if( lcchar( MCPFN, kname,MCPFN+1, &nc ) ){
                  kname[nc] = '\0';
                  if( chdir(kname) != 0 ){
                    *nerr = 124;
                    goto L_8888;
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

