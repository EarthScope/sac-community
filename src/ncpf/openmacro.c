
#include <stdio.h>

#include "ncpf.h"
#include "cpf.h"


#include "co.h"
#include "vars.h"

#define	MVARSSIZE	200

void /*FUNCTION*/ openmacro(kmacroname, kmacroname_s, kmacroargs, 
	 kmacroargs_s, nerr)
char *kmacroname;   int kmacroname_s;
char *kmacroargs;   int kmacroargs_s;
int *nerr;
{
	int notused;
        FILE *nun;


	/*=====================================================================
	 * PURPOSE: To open a SAC macro (command) file.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    kmacroname:   Name of the macro. [c]
	 *    kmacroargs:   Argument text string if any. [c]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    kvarsname:    Name of the vars file used during macro processing.
	 *    nerr:    Error return flag.  Set to 0 if no error occurred. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  cpf/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    MCPW
	 *    cpf:     nmacrolevel
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    cpf:     kvarsname
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  setmsg, apcmsg, apimsg, createvlist, zgtfun, zopen, 
	 *             putvvinteger, macropreamble
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    nun   :     Fortran file unit used in opening macro file. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900206   Added recursion and unlimited macro nesting logic.
	 *    870402:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900129
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Create the vars section used to store fileunit and macro arguments. */

        sprintf(kmcpf.kvarsname,"macro%3.3d", cmcpf.nmacrolevel );

	createvlist( kmcpf.kvarsname,9, MVARSSIZE, &notused, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Open macro file. */

	zopens( &nun, kmacroname,kmacroname_s, "TEXT",5, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Store the macro name and fortran file unit number in the vars section. */

	putvvstring( kmcpf.kvarsname,9, "macroname",10, 0, kmacroname,kmacroname_s, 
	 nerr );
	if( *nerr != 0 )
		goto L_8888;

	putvFILEptr( kmcpf.kvarsname,9, "fileunit",9, nun, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - A SAC macro file contains an optional preamble and a body. */

	/* - Process the preamble.
	 *   The preamble defines the keyword list and optional default values.
	 *   Each preamble line begins with a dollar sign. */

	macropreamble( kmacroargs,kmacroargs_s, nun, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - The body immediately follows the preamble.
	 *   It is processed by successive calls to the logical function macroline. */

L_8888:
	return;

} /* end of function */

