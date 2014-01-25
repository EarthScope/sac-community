
#include "gd2.h"
#include "debug.h"

void /*FUNCTION*/ cursor2(xloc, yloc, kchar, len)
float *xloc, *yloc;
char *kchar;
int len;
{
  
	/*=====================================================================
	 * PURPOSE:  To perform "graphics input function on device 2 (SGF).
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    XLOC:    X location of cursor when character was struck. [f]
	 *    YLOC:    Y location of cursor. [f]
	 *    KCHAR:   Character struck in response to cursor. [c1]
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *===================================================================== */
	/* PROCEDURE: */
	/* - This is a no-op for this graphics device. */
  UNUSED(len);
  UNUSED(kchar);
  UNUSED(yloc);
  UNUSED(xloc);

	return;


} /* end of function */

