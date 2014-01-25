
#include "gd2.h"

void 
draw2(float xloc, float yloc)
{
	int ixloc, iyloc, nerr;



	/*=====================================================================
	 * PURPOSE:  To perform DRAW operation on graphics device 2 (SGF).
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    XLOC:    X viewport location. [f]
	 *    YLOC:    Y viewport location. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GD2:     JFBMAX
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GD2:     MFBUF, JFBPNT
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861016:  Original version.
	 *=====================================================================
	 * DOCUMENTED:  861016
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Scale floating point values to the devices coordinates. */
	ixloc = xloc*XW;
	iyloc = yloc*XW;

	/* - Store location in buffer. */

	Mfbuf[cmgd2.jfbpnt] = ixloc;
	Mfbuf[cmgd2.jfbpnt + 1] = iyloc;
	cmgd2.jfbpnt = cmgd2.jfbpnt + 2;

	/* - Flush buffer if necessary. */

	if( cmgd2.jfbpnt > JFBMAX )
		flushbuffer2( &nerr );

       
	return;

} /* end of function */


void
drawpoly2(float *x, float *y, int n) {
  int j;
  move2(x[0], y[0]);
  for(j = 1; j < n; j++) {
    draw2(x[j], y[j]);
  }
}

void
fillpoly2(float *x, float *y, int n) {
  int nerr;
  int j;
  move2(x[0], y[0]);
  for(j = 1; j < n; j++) {
    draw2(x[j], y[j]);
  }
  Mfbuf[cmgd2.jfbpnt] = MOPFILLRGB;
  cmgd2.jfbpnt += 1;

	if( cmgd2.jfbpnt > JFBMAX )
		flushbuffer2( &nerr );
  
}
