/** 
 * @file   xnews.c
 * 
 * @brief  NEWS command
 * 
 */

#include <stdio.h>
#include <string.h>

#include "exm.h"
#include "co.h"
#include "msg.h"
#include "bool.h"

#include "string_utils.h"


#include "gdm.h"
#include "bot.h"

/** 
 * Execute the NEWS command which writes the news to the users terminal
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   920416:  Modified to print blank lines in the news file.
 * @date   900430:  Changed method of determining number of lines on screen.
 * @date   890105:  Modified from terminal to message subsystem output.
 * @date   870923:  Deleted ".saf" from aux file names.
 * @date   831110:  Generalized pathname generation of NEWS file.
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   810115:  Major revision for new help package format.
 * @date   800915:  Original version.
 *
 */
void 
xnews(int *nerr) {

	char erasechars[9], kfile[MCPFN+1], kresp[9], ktext[MCMSG+1];
        int  idx ;
	int nc, nlinesscreen, nlw, noerr, numchar;
        FILE *nun;

	*nerr = 0;

        for( idx = 0 ; idx < MCPFN ; idx++ )
            kfile[ idx ] = ' ' ;
        kfile[ MCPFN ] = '\0' ;

	/* - Determine number of lines on the screen. */

	getalphainfo( &nlinesscreen, erasechars,9 );

	/* - Create pathname of NEWS file. */

	zbasename( kfile,MCPFN+1 );
	crname( kfile,MCPFN+1, KDIRDL, "news",5, nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Open file if it exists, send "cute" message if it does not. */

	zopens( &nun, kfile,MCPFN+1, "ROTEXT",7, &noerr );
	if( noerr != 0 ){
	    setmsg( "OUTPUT", 1110 );
	    outmsg();
	    goto L_8888;
	}

	/* - Activate automatic output message mode. */

	autooutmsg( TRUE );
	setmsg( "OUTPUT", 99 );

	/* - Read NEWS file and echo to terminal.
	 *   Pause after each full screen and query terminal
	 *   to see it more is desired. */

	nlw = 0;
L_3000:
        if(fgetsp(ktext,MCMSG+1,nun) == NULL) goto L_6000;
        if(ktext[(numchar=strlen(ktext)-1)] == '\n')ktext[numchar] = '\0';

	nc = indexb( ktext,MCMSG+1 );
	if( nc == 0 ){
	    fstrncpy( ktext, MCMSG, " ", 1 );
	}
	aplmsg( ktext,MCMSG+1 );
	nlw = nlw + 1;
	if( nlw >= (nlinesscreen - 2) ){
	    outmsg();
	    clrmsg();
	    zgtmsg( "More? $",8, kresp,9 );
	    upcase( kresp, 1, kresp,9 );
	    if( kresp[0] == 'N' ){
		goto L_6000;
	    }
	    else if( kresp[0] == 'Q' ){
		goto L_6000;
	    }
	    else{
		nlw = 0;
		setmsg( "OUTPUT", 99 );
	    }
	}
	goto L_3000;

	/* - Close NEWS file and deactivate automatic output mode. */

L_6000:
	zcloses( &nun, nerr );
	autooutmsg( FALSE );

L_8888:
	*nerr = 0;
	return;

} /* end of function */

