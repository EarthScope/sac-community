/** 
 * @file   xmacro.c
 * 
 * @brief  MACRO command
 * 
 */

#include <string.h>

#include "exm.h"
#include "msg.h"


#include "co.h"
#include "bot.h"
#include "cpf.h"
#include "ncpf.h"

/** 
 * Execute the MACRO command which execute a macro command file
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   900207:  Added local temporary storage of macro information.
 * @date   880404:  Deleted forced lowercasing of macro arguments.
 * @date   871222:  Moved file inquire to zinquire.
 * @date   870415:  Original version.
 *
 */
void 
xmacro(int *nerr) {

	char ktempargs[MCMSG+1], ktempmacro[MCPFN+1], ktempreqmacro[MCPFN+1],
	     kvarsname[9] = "        " ;
	int lexist;
	int j, idx,  nc, nmacrolevel;

	*nerr = 0;
	for( idx = 0 ; idx < MCMSG ; idx++ )
	    ktempargs[ idx ] = ' ' ;
	ktempargs[0] = '\0' ;	/* for lines fix access violation, maf 980507 */
	ktempargs[ MCMSG ] = '\0' ;
	for( idx = 0 ; idx < MCPFN ; idx++ ){
	    ktempmacro[ idx ] = 0 ;
	    ktempreqmacro[ idx ] = 0 ;
	}
	ktempmacro[0] = '\0' ;
	ktempmacro[ MCPFN ] = '\0' ;
	ktempreqmacro[0] = '\0' ;
	ktempreqmacro[ MCPFN ] = '\0' ;
	kvarsname[0] = '\0' ;
	

	/*   PARSING PHASE: */

	/* - First token is the name of the macro.
	 *   Remainder of command becomes the arguments to the macro.
	 * - Restore previous values if no command arguements were entered. */

	if( lcchar( MCPFN, ktempreqmacro,MCPFN+1, &nc ) ){
    //lcrest( MCMSG, ktempargs,MCMSG+1, &nc );
	}
	else{
	    strcpy( ktempreqmacro, kmexm.kmcreq );
	    strcpy( ktempargs, kmexm.kargs );
	}

	/*   EXECUTION PHASE: */

	/* - Search for the macro file.  Search order is: */

	/*   (1) current directory.
	 *   (2) user specified directories (see SETMACRO command.)
	 *   (3) global macro directory. */

	strcpy( ktempmacro, ktempreqmacro );
	zinquire( ktempmacro, &lexist );
	if( lexist )
	    goto L_5000;

	for( j = 0; j < cmexm.nmcdir; j++ ){
	    strcpy( ktempmacro, kmexm.kmcdir[j] );
	    crname( ktempmacro,MCPFN+1, KDIRDL, ktempreqmacro,MCPFN+1, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	    zinquire( ktempmacro, &lexist );
	    if( lexist )
		goto L_5000;
	}

	zbasename( ktempmacro,MCPFN+1 );
	crname( ktempmacro,MCPFN+1, KSUBDL, "macros",7, nerr );
	if( *nerr != 0 )
	    goto L_8888;
	crname( ktempmacro,MCPFN+1, KDIRDL, ktempreqmacro,MCPFN+1, nerr );
	if( *nerr != 0 )
	    goto L_8888;
	zinquire( ktempmacro, &lexist );
	if( lexist )
	    goto L_5000;

	/* - Raise error condition if macro file does not exist. */

	*nerr = 108;
	setmsg( "ERROR", *nerr );
	apcmsg( ktempreqmacro,MCPFN+1 );
	goto L_8888;

	/* - If this is the top level macro,
	 *   save information on name of macro and arguments. */

L_5000:

	getmacroinfo( &nmacrolevel, kvarsname,9 );
	if( nmacrolevel == 0 ){
	    strcpy( kmexm.kmcreq, ktempreqmacro );
	    strcpy( kmexm.kargs, ktempargs );
	}

	/* - Execute the macro file. */

	executemacro( ktempmacro,MCPFN+1, ktempargs,MCMSG+1, nerr );

L_8888:
	return;

} /* end of function */

