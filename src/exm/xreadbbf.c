/** 
 * @file   xreadbbf.c
 * 
 * @brief  READBBF command
 * 
 */

#include "exm.h"
#include "bbs.h"
#include "vars.h"


#include "cpf.h"

/** 
 * Execute the READBBF command which reads a blackboard variable file
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   890324:  Added call to deletevlist prior to getvlist.
 * @date   890309:  Changed from call to readvfile to getvlist.
 * @date   890227:  Changed due to modified vars subroutines.
 * @date   870916:  Made fully operational.
 * @date   870301:  Original version.
 *
 */
void 
xreadbbf(int *nerr) {

	int notused;

	*nerr = 0;

	deletevlist( kmbbs.knmbbs,MCPFN+1, "MEMORY", nerr );
	if( *nerr != 0 )
		goto L_8888;

L_1000:
	if( lcmore( nerr ) ){

		/* -- "name":  the name of the global variable file. */
		if( lcchar( MCPFN, kmbbs.knmbbs,MCPFN+1, &notused ) ){

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

  /* Recreate the variable group */
  sac_vars_create(kmbbs.knmbbs);

	/* - Get (read from disk) the new blackboard list. */
  readvfile(kmbbs.knmbbs, MCPFN+1, &notused, nerr);

L_8888:
	return;

}

