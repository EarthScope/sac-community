
#include "gd2.h"
#include "bool.h"
#include "debug.h"


void /*FUNCTION*/ getwindowstat2(number, exists)
int number;
int *exists;
{
  UNUSED(number);
	/*=====================================================================
	 * PURPOSE: To get graphics window attributes for device 2 (SGF.)
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    NUMBER:  The number of the graphic window. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    EXISTS:  Set to .TRUE. if graphics window exists. [l]
	 *             Potential error numbers: 0201.
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861201:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850506
	 *===================================================================== */
	/* PROCEDURE: */
	/* - This is a no-op for this graphics device. */
	*exists = FALSE;

       
	return;

} /* end of function */

