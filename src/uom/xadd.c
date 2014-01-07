
#include "uom.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xadd(nerr)
int *nerr;
{
	int lchn;
	int j, jcon, jdfl;
	double con;

  sac *s;
	/* Ind
	 *=====================================================================
	 * PURPOSE:  To execute the action command ADD.
	 *           This command adds a constant to data in memory.
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
	 *    UOM:     CONADD()
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    UOM:     CONADD()
	 *    HDR:     DEPMIN, DEPMAX, DEPMEM
	 *=====================================================================
	 * ADDROUTINES CALLED:
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

		/* -- "v":  constant to add. */
		if( lcreal( &con ) ){
			jcon = jcon + 1;
			Conadd[jcon] = con;
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
				Conadd[j] = con;
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

	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){

		/* -- Get next file from memory manager. */
    if(!(s = sacget(jdfl-1, TRUE, nerr))) {
      goto L_8888;
    }
		//getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

		/* -- Add appropriate constant to each data point. */
		con = Conadd[jdfl];
    for( j = 0; j < s->h->npts; j++ ){
      s->y[j] += con;
    }

		/* -- Recompute extrema. */
		s->h->depmen = s->h->depmen + con;
		s->h->depmin = s->h->depmin + con;
		s->h->depmax = s->h->depmax + con;

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

