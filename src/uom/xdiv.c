
#include "uom.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "msg.h"
#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xdiv(nerr)
int *nerr;
{
	int lchn;
	int j, jcon, jdfl;
	double con, temp;

  sac *s;

	/*=====================================================================
	 * PURPOSE:  To execute the action command DIV.
	 *           This command divides a constant into data in memory.
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
	 *    UOM:     CONDIV()
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    UOM:     CONDIV()
	 *    HDR:     DEPMIN, DEPMAX, DEPMEM
	 *=====================================================================
	 * DIVROUTINES CALLED:
	 *    SACLIB:  LCMORE, LCREAL, CFMT, CRESP,
	 *             VFLIST, VFTIME, GETFIL, PUTFIL
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    CON:     Constant currently being used in division.
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
			if( con != 0. ){
				Condiv[jcon] = con;
				}
			else{
				*nerr = 1701;
				setmsg( "ERROR", *nerr );
				goto L_8888;
				}
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
			Condiv[j] = con;
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

		/* -- Divide appropriate constant into each data point.
		 *    (Multiply by reciprocal since this is faster.) */
		con = 1./Condiv[jdfl];

		for( j = 0; j < s->h->npts; j++ ){
      s->y[j] *= con;
    }

		/* -- Recompute extrema. */
		s->h->depmen = s->h->depmen*con;
		if( con >= 0. ){
			s->h->depmin = s->h->depmin*con;
			s->h->depmax = s->h->depmax*con;
			}
		else{
			temp = s->h->depmin;
			s->h->depmin = s->h->depmax*con;
			s->h->depmax = temp*con;
			}

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

