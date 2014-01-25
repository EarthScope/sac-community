
#include "gd2.h"
#include "debug.h"

void /*FUNCTION*/ getalphainfo2(nlines, erase, len)
int *nlines;
char *erase;
int len;
{
  UNUSED(len);
  UNUSED(erase);
  UNUSED(nlines);
	/*=====================================================================
	 * PURPOSE: To inquire about text attributes of graphics device 2.
	 *=====================================================================
	 * MODULE/LEVEL: GD2/4
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NLINES:  Number of text lines per screen. [i]
	 *    ERASE:   Text to send to erase terminal screen. [c]
	 *             Set to all blanks is terminal has scrolling capability.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861020:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861020
	 *===================================================================== */
	/* PROCEDURE: */
	/* - This is a no-op for this graphics device. */
       
	return;

} /* end of function */

