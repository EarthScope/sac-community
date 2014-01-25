
#include <stdio.h>

#include "ncpf.h"
#include "cpf.h"


#include "co.h"
#include "vars.h"

void /*FUNCTION*/ closemacro(nerr)
int *nerr;
{
	int ntused;
        FILE *nun;


	/*=====================================================================
	 * PURPOSE: To close a SAC macro (command) file.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:     Error return flag.  Set to 0 if no error occurred. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  cpf/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    MCPW
	 *    cpf:     nmacrolevel, kvarsname
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  getvvinteger, zclose, deletevlist
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    kvarsname:  Name of vars list containing macro information. [k]
	 *    nun:        Fortran file unit used in opening macro file. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900129:  Now passing nesting level not macro name.
	 *    870402:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870402
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Get the fortran file unit number from the vars list. */

	getvFILEptr( kmcpf.kvarsname,9, "fileunit",9, &nun, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Close the macro file and delete the vars list. */

	zcloses( &nun, &ntused );
	deletevlist( kmcpf.kvarsname,9, "MEMORY", nerr );
	if( *nerr != 0 )
		goto L_8888;

L_8888:
	return;

} /* end of function */

