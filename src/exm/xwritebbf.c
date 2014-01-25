/** 
 * @file   xwritebbf.c
 * 
 * @brief  WRITEBBF command
 * 
 */

#include "exm.h"
#include "bbs.h"
#include "cpf.h"


#include "co.h"
#include "vars.h"

/** 
 * Execute the writebbf command which writes the blackboard variable to a file
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   890227:  Changes due to modified vars subroutines.
 * @date   870916:  Made fully operational.
 * @date   870301:  Original version.
 *
 */
void 
xwritebbf(int *nerr) {

	char kname[MCPFN+1];
	int notused;

	*nerr = 0;

	/* - Loop on each token in command: */
	fstrncpy( kname, MCPFN, " ", 1 );
L_1000:
	if( lcmore( nerr ) ){

		/* -- "name":  the name of the global variable file. */
		if( lcchar( MCPFN, kname,MCPFN+1, &notused ) ){

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

	writevfile( kmbbs.knmbbs,MCPFN+1, kname, nerr );

       
	return;

}

