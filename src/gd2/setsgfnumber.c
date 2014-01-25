
#include "gd2.h"

void /*FUNCTION*/ setsgfnumber(number)
int number;
{



	/*=====================================================================
	 * PURPOSE:  To set the frame number to use for
	 *           subsequent SAC Graphics Files (SGF).
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    NUMBER:  Frame number to use for next SGF. [i]
	 *             = 0  Search directory for SGF's and set the next frame
	 *                  number to be the lowest unused one.
	 *             > 0  Set the next frame number to be this value.
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GD2:     NFNUM
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
	cmgd2.nfnum = number;
	cmgd2.lfnum = cmgd2.nfnum <= 0;

       
	return;

} /* end of function */

