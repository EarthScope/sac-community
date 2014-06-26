
#include "sam.h"
#include "amf.h"
#include "dfm.h"
#include "hdr.h"
#include "bool.h"


#include "co.h"
#include "msg.h"
#include "ucf.h"
#include "dbh.h"
#include "dff.h"

#define	MINDATALEN	201
#define	MLENSCRATCH	4297

void /*FUNCTION*/ xhilbert(nerr)
int *nerr;
{
	int jdfl, nlenmn;
  sac *s;
  float *scr;

	/*=====================================================================
	 * PURPOSE: To parse and execute the action command HILBERT.
	 *          This command computes the Hilbert transform.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  sam/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    dfm:     ndfl
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    hdr:     depmin, depmax, depmen
	 *    mem:     sacmem
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  vflist, vfeven, getfil, setmsg, apimsg, allamb, firtrn,
	 *             extrma
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    MLENSCRATCH:  Size of scratch space needed for transform. [ip]
	 *    ndxscratch:   Index in SACMEM array for scratch space. [i]
	 *    ndxsignal:    Index in SACMEM array for current signal. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    890223:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  890223
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* CHECKING PHASE: */

	/* - Test for a non-null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Make sure each file is an evenly spaced time series file. */

	vfeven( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Determine minimum signal size.
	 *   Make sure minimum is not too small for fir filter subroutine. */

	nlenmn = MLARGE;
	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
    if(!(s = sacget(jdfl-1, FALSE, nerr))) {
      goto L_8888;
    }
		//getfil( jdfl, FALSE, &ntused, &ntused, &ntused, nerr );

		nlenmn = min( nlenmn, s->h->npts );
		}

	if( nlenmn < MINDATALEN ){
		*nerr = 1613;
		setmsg( "ERROR", *nerr );
		apimsg( MINDATALEN );
		goto L_8888;
		}

	/* - EXECUTION PHASE: */

	/* - Allocate temporary block for scratch space. */
  scr = (float *) malloc(sizeof(float) * MLENSCRATCH);
	//allamb( &cmmem, MLENSCRATCH, &ndxscratch, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){

    if(!(s = sacget(jdfl-1, TRUE, nerr))) {
      goto L_8888;
    }
		//getfil( jdfl, TRUE, &nlnsignal, &ndxsignal, &notused, nerr );

		/* -- Compute the Hilbert transform in place. */
		firtrn( "HILBERT", s->y, s->h->npts, scr, s->y);

		/* -- Update any header fields that may have changed. */
		extrma( s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax, &s->h->depmen );

		}

	/* - Release scratch space. */
  FREE(scr);

	if( *nerr != 0 )
		goto L_8888;

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

} /* end of function */

