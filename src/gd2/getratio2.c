
#include "gd2.h"

void /*FUNCTION*/ getratio2(aspect)
float *aspect;
{



	/*=====================================================================
	 * PURPOSE:  To inquire about the aspect ratio (ratio of y viewport
	 *           size to x viewport size) for graphics device 2 (SGF).
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    ASPECT:  Aspect ratio. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861020:  Original version.
	 *=====================================================================
	 * DOCUMENTED:  861020
	 *===================================================================== */
	/* PROCEDURE: */
	*aspect = 0.75;

       
	return;

} /* end of function */

