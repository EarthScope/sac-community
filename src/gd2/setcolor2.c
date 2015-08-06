
#include "gd2.h"


GD2_EXTERN

void /*FUNCTION*/
setcolor2(color c) {
    int nerr;

    /*=====================================================================
	 * PURPOSE:  To set the color attribute for graphics device 2 (SGF).
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    INDEX:   Color index number. [i]
	 *             = 0  Use normal of default color.
	 *             > 0  Use a device-specific color.
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GD2:     MOPCOL, JFBMAX
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GD2:     JFBPNT, MFBUF
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  FLUSHBUFFER2
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861020:  Original version.
	 *=====================================================================
	 * DOCUMENTED:  861020
	 *===================================================================== */
    /* PROCEDURE: */
    /* - Add color opcode to file buffer. */
    Mfbuf[cmgd2.jfbpnt] = MOPCOL;
    Mfbuf[cmgd2.jfbpnt + 1] = 3;
    Mfbuf[cmgd2.jfbpnt + 2] = c.r;
    Mfbuf[cmgd2.jfbpnt + 3] = c.g;
    Mfbuf[cmgd2.jfbpnt + 4] = c.b;

    cmgd2.jfbpnt = cmgd2.jfbpnt + 5;

    /* - Flush buffer if necessary. */

    if (cmgd2.jfbpnt > JFBMAX)
        flushbuffer2(&nerr);

    return;

}                               /* end of function */
