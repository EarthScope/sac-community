
#include <string.h>

#include "pl.h"
#include "gem.h"
#include "bool.h"

void /*FUNCTION*/ plsave()
{
	/* ind
	 *=====================================================================
	 * PURPOSE:  To save the current graphics environment.
	 *=====================================================================
	 * MODULE/LEVEL:  GEM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GEM:     MCMGEM, MKMGEM
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GEM:     LGEMS, CMGEMS(), KMGEMS()
	 *=====================================================================
	 * GLOBAL COUPLING:
	 * - PLREST restores graphics environment.
	 * - CMGEMA and KMGEMA are dummy arrays equivalenced to the first
	 *   element in the CMGEM and KMGEM arrays that need to be saved.
	 * - MCMGEM and MKMGEM are the number of elements to be saved.
	 *=====================================================================
	 * NOTE:  The graphic environment parameters that can change are saved
	 * into special scratch arrays by this call.  A call to PLREST reverses
	 * this operation.  This allows each plot command to control the graphic
	 * environment while making it's plot and then restore the environment
	 * to it's original state.
	 *===================================================================== */
	/* PROCEDURE: */


        memcpy((char *)&cmgemsav.lxlim,(char *)&cmgem.lxlim,sizeof (struct t_cmgem));
        memcpy(&kmgemsav.ksides,&kmgem.ksides,sizeof (struct t_kmgem));

        lgems = TRUE;

	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    840118:  Modified method of saving environment.
	 *    820831:  Changes due to modification of GEM common blocks.
	 *===================================================================== */

} /* end of function */

