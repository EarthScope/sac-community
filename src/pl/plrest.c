
#include <string.h>

#include "pl.h"
#include "gem.h"
#include "bool.h"

void /*FUNCTION*/ plrest()
{
	/* ind
	 *=====================================================================
	 * PURPOSE:  To restore the previously saved graphics environment.
	 *=====================================================================
	 * MODULE/LEVEL:  GEM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GEM:     MCMGEM, MKMGEM, CMGEMS(), KMGEMS()
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GEM:     All.
	 *=====================================================================
	 * GLOBAL COUPLING:
	 * - PLSAVE must have been called prior to call to PLREST.
	 *===================================================================== */
	/* PROCEDURE: */


	if( lgems ){
          memcpy((char *)&cmgem.lxlim,(char *)&cmgemsav.lxlim,sizeof (struct t_cmgem));
          memcpy(&kmgem.ksides,&kmgemsav.ksides,sizeof (struct t_kmgem));
          lgems = FALSE;
        }
        
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    840118:  Modified method used to save and restore environment.
	 *    820831:  Changes due to modification of GEM common blocks.
	 *===================================================================== */

} /* end of function */

