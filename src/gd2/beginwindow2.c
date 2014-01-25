

#include "gd2.h"
#include "debug.h"

void /*FUNCTION*/ beginwindow2(number, nerr)
int *number, *nerr;
{

	/*=====================================================================
	 * PURPOSE: To begin plotting to a graphics window for device 2 (SGF).
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    NUMBER:  Graphics window number. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error return flag. [i]
	 *=====================================================================
	 * MODULE/LEVEL: GD2/4
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861104:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861104
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - This is a no-op for this graphics device. */
  UNUSED(number);
  
	return;


} /* end of function */

