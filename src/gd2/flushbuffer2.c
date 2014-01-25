
#include "gd2.h"


#include "co.h"

void /*FUNCTION*/ flushbuffer2(nerr)
int *nerr;
{
	int numw;

	/*=====================================================================
	 * PURPOSE:  To flush the buffer for graphics device 2 (SGF).
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error return flag. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GD2:     MOPNUL, JFBPNT, MFBUF, JFDPNT
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GD2:     JFBPNT, MFBUF, JFDPNT
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  ZWABS
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861014:  Original version based on FFLUSH.
	 *=====================================================================
	 * DOCUMENTED:  861014
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - If buffer is not empty: */

	if( cmgd2.jfbpnt > 1 ){

		/* -- Force an even number of 16-bit integers in the current buffer,
		 *    by appending a NULL opcode if necessary.
		 *    This is needed because ZWABS works with long (32-bit) words. */

		if( (cmgd2.jfbpnt/2)*2 == cmgd2.jfbpnt ){
			Mfbuf[cmgd2.jfbpnt] = MOPNUL;
			}
		else{
			cmgd2.jfbpnt = cmgd2.jfbpnt - 1;
			}
		numw = cmgd2.jfbpnt/2;

		/* -- Store the size of the buffer in the first word being written.
		 *    This is a long integer physically stored immediately before the short
		 *    integer buffer. This length is used to facilitate the reading process. */
		cmgd2.nfbuf = numw;

		/* -- Write the buffer to disk, including the length of the buffer. */
		zwabs( (int *)&cmgd2.jfun, (char *)(&cmgd2.nfbuf), (numw + 1), (int *)&cmgd2.jfdpnt, (int *)nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Update the disk and buffer pointers. */
		cmgd2.jfdpnt = cmgd2.jfdpnt + numw + 1;
		cmgd2.jfbpnt = 1;
		}

L_8888:
	return;

} /* end of function */

