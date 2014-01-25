
#include "gd2.h"
#include "debug.h"

void /*FUNCTION*/ cursortext2(xloc, yloc, ktext, len)
float *xloc, *yloc;
char *ktext;
int len;
{

	/*=====================================================================
	 * PURPOSE:  To perform "cursor text input function on device 2 (SGF.)
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    XLOC:    X location of cursor when character was struck. [f]
	 *    YLOC:    Y location of cursor. [f]
	 *    KTEXT:   Text entered in response to cursor. [c]
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    870323:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870323
	 *===================================================================== */
	/* PROCEDURE: */
	/* - This is a no-op for this device-driver. */
  UNUSED(len);
  UNUSED(ktext);
  UNUSED(yloc);
  UNUSED(xloc);
	return;

} /* end of function */

