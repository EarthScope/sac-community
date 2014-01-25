
#include "gd2.h"

void /*FUNCTION*/ getdevicerat2(ratio)
float *ratio;
{

	/*=====================================================================
	 * PURPOSE:  To inquire about the graphics screen aspect ratio.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    RATIO:   Aspect ratio of graphics device 2. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861026:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861026
	 *===================================================================== */
	/* PROCEDURE: */
	*ratio = 0.75;

       
	return;

} /* end of function */

