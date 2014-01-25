
#include <string.h>

#include "mach.h"
#include "ncpf.h"
#include "cpf.h"
#include "bool.h"

#include "bot.h"
#include "co.h"

void /*FUNCTION*/ setmacrostatus(kstatus, kstatus_s)
char *kstatus;   int kstatus_s;
{
	char ktemp[9];
	int nc;



	/*=====================================================================
	 * PURPOSE: To set the status of a SAC macro (command) file.
	 *=====================================================================
	 * INTPUT ARGUMENTS:
	 *    status:   (Execution) status of SAC macro. [c]
	 *              = 'OK' if everything is going well.
	 *              = 'ERROR' if an execution error has occurred. 
	 *=====================================================================
	 * MODULE/LEVEL:  cpf/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    MCPW
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    cpf:     lmacrostatus
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900207:  Changed status variable to a logical variable.
	 *    870722:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870722
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Convert input to upper case and copy to local variable. */
	nc = min( MCPW, indexb( kstatus,kstatus_s ) );
	modcase( TRUE, kstatus, nc, ktemp );

	/* - Set status variable to appropriate state. */

	if( memcmp(ktemp,"OK",2) == 0 ){
		cmcpf.lmacrostatus = TRUE;
		}
	else if( memcmp(ktemp,"ERROR",5) == 0 ){
		cmcpf.lmacrostatus = FALSE;
		}
	else{
		cmcpf.lmacrostatus = TRUE;
		}

       
	return;

} /* end of function */

