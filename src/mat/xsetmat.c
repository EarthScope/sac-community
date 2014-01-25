
#include <config.h>
#include "debug.h"

#ifdef HAVE_MATLAB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mach.h"
#include "bool.h"
#include "mat.h"

char matdir[MCPFN+1];

void /*FUNCTION*/ xsetmat(nerr)
int *nerr;
{
	char ktemp[MCPFN+1];
	int nchar; 

	/*=====================================================================
	 * PURPOSE:  To execute the action command SETMAT.
	 *           This command sets the matlab search path.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag
	 *=====================================================================
	 * MODULE/LEVEL:  MAT/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *         MATDIR
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LCCHAR
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    970514:  Original version. Based on xsetmacro.  maf
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;


	/* - Get token */


	/* -- "text":  the name of a directory to search for matlab scripts. */
	if( lcchar( MCPFN, ktemp,MCPFN+1, &nchar ) ){
		if( MODEFILECASE < 0 ){
		    modcase( FALSE, ktemp, nchar, (char*)matdir );
		}
		else if( MODEFILECASE > 0 ){
		    modcase( TRUE, ktemp, nchar, (char*)matdir );
		}
		else{
		    strcpy( matdir, ktemp );
		    matdir[MCPFN]='\0';
		}
	} /* end if( lcchar( MCPFN, ktemp,MCPFN+1, &nchar ) ) */

	/* -- Bad syntax. */
	else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	}

	if ( lcmore( nerr ) ) {
		setmsg ( "WARNING" , 8001 ) ;
		outmsg () ;
		clrmsg () ;
	}

	return;

} /* end of function */


#endif /* HAVE_MATLAB */


#ifndef HAVE_MATLAB

void matlab_unavailable();

void xsetmat(int *err) { UNUSED(err); matlab_unavailable(); }

#endif

