
#include "uom.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xsub(nerr)
int *nerr;
{
	int lchn;
	int j, jcon, jdfl, ndx1, ndx2, nlen;
	double con;

	float *Sacmem;

	/* Ind
	 *=====================================================================
	 * PURPOSE:  To execute the action command SUB.
	 *           This command subtracts a constant from data in memory.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  UOM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *    UOM:     CONSUB()
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    UOM:     CONSUB()
	 *    HDR:     DEPMIN, DEPMAX, DEPMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, LCREAL, CFMT, CRESP,
	 *             VFLIST, VFTIME, GETFIL, PUTFIL
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    CON:     Constant currently being used in subtraction.
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	jcon = 0;
	lchn = FALSE;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "v":  constant to subtract. */
		if( lcreal( &con ) ){
			jcon = jcon + 1;
			Consub[jcon] = con;
			lchn = TRUE;

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
		goto L_8888;

	/* - Fill remainder of array with last input constant. */

	if( lchn ){
		for( j = jcon + 1; j <= MDFL; j++ ){
			Consub[j] = con;
			}
		}

	/* CHECKING PHASE: */

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Check to make sure all files are time series files. */

	vftime( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Get next file from memory manager. */
		getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Subtract appropriate constant from each data point. */
		con = Consub[jdfl];
                Sacmem = cmmem.sacmem[ndx1];
		for( j = ndx1; j <= (ndx1 + nlen - 1); j++ ){
                        *(Sacmem++) -= con;
			}

		/* -- Recompute extrema. */
		*depmen = *depmen - con;
		*depmin = *depmin - con;
		*depmax = *depmax - con;

		/* -- Give file back to memory manager. */
		putfil( jdfl, nerr );
		if( *nerr != 0 )
			goto L_8888;
		}

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    820701:  Documented subroutine.
	 *    820701:  Changed to newest set of parsing and checking functions.
	 *===================================================================== */

} /* end of function */

