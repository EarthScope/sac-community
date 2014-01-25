
#include "gd2.h"

void /*FUNCTION*/ setlinestyle2(iline)
int *iline;
{
	int nerr;



	/*=====================================================================
	 * PURPOSE:  To change the linestyle for graphics device 2 (SGF).
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    ILINE:   Linestyle attribute. [i]
	 *             = 0 for NO line to be drawn by GLINE.
	 *             = 1 for a solid line.
	 *             > 1 for device-specific linestyles.
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GD2:     MOPLIN
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  FLUSHBUFFER2
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861020:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861020
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Write linestyle opcode to SGF buffer. */
	Mfbuf[cmgd2.jfbpnt] = MOPLIN;
	Mfbuf[cmgd2.jfbpnt + 1] = 1;
	Mfbuf[cmgd2.jfbpnt + 2] = *iline;
	cmgd2.jfbpnt = cmgd2.jfbpnt + 3;

	/* - Flush buffer if necessary. */

	if( cmgd2.jfbpnt > JFBMAX )
		flushbuffer2( &nerr );

       
	return;

} /* end of function */

