
#include "gd2.h"
#include "gdm.h"
#include "gam.h"
#include "debug.h"

#include "sgfcolor.h"

void 
setctable2(int          iwindow, 
           unsigned int nentry, 
           float        red[], 
           float        green[], 
           float        blue[] )
{
  UNUSED(iwindow);
  UNUSED(nentry);
  UNUSED(blue);
  UNUSED(green);
  UNUSED(red);
	/*=====================================================================
	 * PURPOSE:  To set the color table for graphics device 2 (SGF).
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    IWINDOW: Graphics window number. [i]
	 *    NENTRY:  Number of entries in the color table. [i]
	 *    RED:     Array of red color values. [fa]
	 *    GREEN:   Array of green color values. [fa]
	 *    BLUE:    Array of blue color values. [fa]
	 *=====================================================================
	 * SEE SETCTABLE subroutine documentation for important information
	 *               about the format and use of these color tables.
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861020:  Original version.
	 *=====================================================================
	 * DOCUMENTED:  861020
	 *===================================================================== */
	/* PROCEDURE: */

        
    int  i;

    for (i = 0; i < cmgdm.npscimage; i++){

      sred[i] = sgfred[i];
      sgreen[i] = sgfgreen[i];
      sblue[i] = sgfblue[i];
    }
  cmgam.cmap = MCOLOR;

	return;

} /* end of function */

