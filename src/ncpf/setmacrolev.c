
#include <stdio.h>

#include "ncpf.h"
#include "cpf.h"
#include "co.h"

void /*FUNCTION*/ setmacrolevel(imacrolevel)
int imacrolevel;
{

	/*=====================================================================
	 * PURPOSE:  To set the macro (nesting) level.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    imacrolevel:  Macro nesting level. [i]
	 *                  = 0 for terminal input. 
	 *                  > 0 for levels of macro nesting.
	 *=====================================================================
	 * MODULE/LEVEL:    cpf/4
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    cpf:   nmacrolevel, kvarsname
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900207:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900207
	 *===================================================================== */
	/* - Store input value in common block. */
	cmcpf.nmacrolevel = max( imacrolevel, 0 );

	/* - Create next name for vars list to store macro information. */

        sprintf(kmcpf.kvarsname,"macro%3.3d",cmcpf.nmacrolevel );

       
	return;

} /* end of function */

