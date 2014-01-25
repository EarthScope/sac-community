
#include <string.h>

#include "co.h"
#include "gd2.h"
#include "bool.h"
#include "gtm.h"
#include "gdm.h"

void 
move2(float xloc, float yloc)
{
	int ixloc, iyloc, nerr;
	float unused, xfactor, xpsize, xvpmax, xvpmin, xvsmax, xvsmin, 
	 xwcmax, xwcmin;



	/*=====================================================================
	 * PURPOSE:  To perform MOVE operation on graphics device 2 (SGF).
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    xloc:    X viewport location. [f]
	 *    yloc:    Y viewport location. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  gd2/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    gd2:     encodesize, MOPMOV, JFBMAX, XW
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gd2:     encodesize, mfbuf, jfbpnt
	 *=====================================================================
	 * GLOBAL COUPLING:
	 * - beginframe2 sets encodesize which is acted upon and cleared here.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900310:  Added logic to encode plot size.
	 *    861016:  Original version.
	 *=====================================================================
	 * DOCUMENTED:  861016
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Encode plot size if necessary. */
	if( cmgd2.encodesize ){
		getvport( &xvpmin, &xvpmax, &unused, &unused );
		getvspace( &xvsmin, &xvsmax, &unused, &unused );
		xfactor = (xvsmax - xvsmin)/(xvpmax - xvpmin);
		if( strcmp(kmgd2.sizetype,"FIXED   ") == 0 ){
			xpsize = cmgd2.sizevalue*xfactor;
			}
		else if( strcmp(kmgd2.sizetype,"SCALED  ") == 0 ){
			getworld( &xwcmin, &xwcmax, &unused, &unused );
			xpsize = cmgd2.sizevalue*(xwcmax - xwcmin)*xfactor;
			}
		else{
			xpsize = 10.0;
			}
		Mfbuf[cmgd2.jfbpnt] = MOPSIZ;
		Mfbuf[cmgd2.jfbpnt + 1] = 1;
		Mfbuf[cmgd2.jfbpnt + 2] = (int)( fmin( 100.*xpsize, XW ) );
		cmgd2.jfbpnt = cmgd2.jfbpnt + 3;
		cmgd2.encodesize = FALSE;
		}

	/* - Scale floating point values to the devices coordinates. */

	ixloc = xloc*XW;
	iyloc = yloc*XW;

	/* - Store MOVE opcode and location in buffer. */

	Mfbuf[cmgd2.jfbpnt] = MOPMOV;
	Mfbuf[cmgd2.jfbpnt + 1] = 2;
	Mfbuf[cmgd2.jfbpnt + 2] = ixloc;
	Mfbuf[cmgd2.jfbpnt + 3] = iyloc;
	cmgd2.jfbpnt = cmgd2.jfbpnt + 4;

	/* - Flush buffer if necessary. */

	if( cmgd2.jfbpnt > JFBMAX )
		flushbuffer2( &nerr );

       
	return;

} /* end of function */

