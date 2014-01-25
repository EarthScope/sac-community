
#include "gd2.h"


#include "co.h"

void /*FUNCTION*/ endframe2(nerr)
int *nerr;
{



	/*=====================================================================
	 * PURPOSE:  To end the current frame for graphics device 2 (SGF).
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error return flag. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GD2:     MOPDON, JFUN
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GD2:     MFBUF, JBFPNT, NFNUM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  FLUSHBUFFER2, ZCLOSE
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861014:  Original version.
	 *=====================================================================
	 * DOCUMENTED:  861014
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Append "Done" opcode to buffer. */

	Mfbuf[cmgd2.jfbpnt] = MOPDON;
	Mfbuf[cmgd2.jfbpnt + 1] = 0;
	cmgd2.jfbpnt = cmgd2.jfbpnt + 2;

	/* - Flush buffer. */

	flushbuffer2( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Close frame file. */

	zclose( &cmgd2.jfun, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Increment frame number if it is not set to be overwritten. */
	if ( !cmgd2.lover )
	    cmgd2.nfnum = cmgd2.nfnum + 1;

L_8888:
	return;

} /* end of function */

