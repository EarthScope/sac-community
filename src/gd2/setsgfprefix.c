
#include <string.h>

#include "gd2.h"


#include "co.h"
#include "bot.h"

void /*FUNCTION*/ setsgfprefix(prefix)
char *prefix;
{



	/*=====================================================================
	 * PURPOSE:  To set the prefix to use for
	 *           subsequent SAC Graphics Files (SGF).
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    PREFIX:  Prefix to use for subsequent SGF's. [c]
	 *             A blank implies the default prefix ("F").
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GD2:     KFNAMB, NFNAMB
	 *=====================================================================
	 * GLOBAL COUPLING:
	 * - Any errors resulting from a bad prefix or frame number will be
	 *   be detected and reported by BEGINFRAME when the SGF is created.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861020:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861020
	 *===================================================================== */
	/* PROCEDURE: */
	fstrncpy( kmgd2.kfnamb, MCPFN, prefix, strlen(prefix));
	cmgd2.nfnamb = indexb( kmgd2.kfnamb,MCPFN+1 );

	if( cmgd2.nfnamb <= 0 ){
		fstrncpy( kmgd2.kfnamb, MCPFN, "F", 1);
		cmgd2.nfnamb = 1;
		}

       
	return;

} /* end of function */

