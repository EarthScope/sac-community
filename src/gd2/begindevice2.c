
#include "gd2.h"
#include "gem.h"

void /*FUNCTION*/ begindevice2(nerr)
int *nerr;
{

	/*=====================================================================
	 * PURPOSE:  To begin plotting to graphics device 2 (SGF).
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error return flag. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861014:  Original version.
	 *=====================================================================
	 * DOCUMENTED:  861014
	 *===================================================================== */
	/* PROCEDURE: */
	/* - This subroutine is a no-op. */
	*nerr = 0;

        set_skeleton_fudge( 0.0003 );

	return;

} /* end of function */

