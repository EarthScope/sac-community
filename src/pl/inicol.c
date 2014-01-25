
#include "pl.h"

void /*FUNCTION*/ inicol(iicol, nicol)
int iicol[], *nicol;
{
	int j;

	int *const Iicol = &iicol[0] - 1;


	/*=====================================================================
	 * PURPOSE:  To initialize the "standard" color list.
	 *           This is the list of colors used in plotting data.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    IICOL:   List of "standard" colors.
	 *    NICOL:   Length of IICOL.
	 *=====================================================================
	 * MODULE/LEVEL:  GEM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Set up standard color table. */
	*nicol = 6;
	for( j = 1; j <= *nicol; j++ ){
		Iicol[j] = j;
		}

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    821221:  Original version from INIGEM.
	 *===================================================================== */

} /* end of function */

