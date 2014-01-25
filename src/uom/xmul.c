
#include "uom.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xmul(nerr)
int *nerr;
{
	int lchn;
	int j, jcon, jdfl, ndx1, ndx2, nlen;
	double con, temp;

	float *Sacmem;

	/* Ind
	 *=====================================================================
	 * PURPOSE:  To execute the action command MUL.
	 *           This command multiplies a constant from data in memory.
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
	 *    UOM:     CONMUL()
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    UOM:     CONMUL()
	 *    HDR:     DEPMIN, DEPMAX, DEPMEM
	 *=====================================================================
	 * MULROUTINES CALLED:
	 *    SACLIB:  LCMORE, LCREAL, CFMT, CRESP,
	 *             VFLIST, VFTIME, GETFIL, PUTFIL
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    CON:     Constant currently being used in multiplication.
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	jcon = 0;
	lchn = FALSE;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "v":  constant to multiply. */
		if( lcreal( &con ) ){
			jcon = jcon + 1;
			Conmul[jcon] = con;
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
			Conmul[j] = con;
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

		/* -- Multiply appropriate constant by each data point. */
		con = Conmul[jdfl];

                Sacmem = cmmem.sacmem[ndx1];
		for( j = ndx1; j <= (ndx1 + nlen - 1); j++ ){
                        *(Sacmem++) *= con;
			}

		/* -- Recompute extrema. */
		*depmen = *depmen*con;
		if( con >= 0. ){
			*depmin = *depmin*con;
			*depmax = *depmax*con;
			}
		else{
			temp = *depmin;
			*depmin = *depmax*con;
			*depmax = temp*con;
			}

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
	 *    830623:  Fixed bug involving recomputing extrema.
	 *    820701:  Documented subroutine.
	 *    820701:  Changed to newest set of parsing and checking functions.
	 *===================================================================== */

} /* end of function */

