/** 
 * @file   xload.c
 * 
 * @brief  LOAD command
 * 
 */

#include <string.h>

#include "exm.h"
#include "comlists.h"
#include "msg.h"
#include "bool.h"


#include "co.h"
#include "wild.h"
#include "bot.h"
#include "cpf.h"

/** 
 * Execute the LOAD command which dynamically loads an external SAC command
 *    into memory
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   900804:  Original version.
 *
 */
void 
xload(int *nerr) {
	char kabbrev[9], kcommand[9], kdirpart[MCPFN+1], kfilepart[MCPFN+1], 
	 kname[MCPFN+1];
	int index, nc;

	*nerr = 0;

	index = 0;
L_1000:
	if( lcmore( nerr ) ){

		/* -- ABBREV text: abbreviation for last external command. */
		if( lkchar( "ABBREV#$",9, MCPW, kabbrev,9, &nc ) ){
			modcase( TRUE, kabbrev, MCPW, kabbrev );
			if( index != 0 && cmcomlists.nextcomnames <= MEXTCOMNAMES ){
				cmcomlists.nextcomnames = cmcomlists.nextcomnames + 
				 1;
				strcpy( kmcomlists.kextcomnames[cmcomlists.nextcomnames - 1], kabbrev
				  );
				Iextcomindex[cmcomlists.nextcomnames] = index;
				}
			else{
				*nerr = 1027;
				setmsg( "ERROR", *nerr );
				apimsg( MEXTCOMNAMES );
				goto L_8888;
				}

			/* -- name: name of external command to load. May include directory path. */
			}
		else if( lcchar( MCPFN, kname,MCPFN+1, &nc ) ){
			zload( kname, &index, nerr );
			if( *nerr != 0 )
				goto L_8888;
			getdir( kname,MCPFN+1, kdirpart,MCPFN+1, kfilepart,MCPFN+1 );
			modcase( TRUE, kfilepart, MCPW, kcommand );
			if( cmcomlists.nextcomnames <= MEXTCOMNAMES ){
				cmcomlists.nextcomnames = cmcomlists.nextcomnames + 
				 1;
				strcpy( kmcomlists.kextcomnames[cmcomlists.nextcomnames - 1], kcommand
				  );
				Iextcomindex[cmcomlists.nextcomnames] = index;
				}
			else{
				*nerr = 1027;
				setmsg( "ERROR", *nerr );
				apimsg( MEXTCOMNAMES );
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

