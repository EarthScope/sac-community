
#include <math.h>

#include "uom.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "ucf.h"
#include "dff.h"

void /*FUNCTION*/ xexp(nerr)
int *nerr;
{
	int j, jdfl;
  sac *s;

	/*=====================================================================
	 * PURPOSE: To parse and execute the action command EXP.
	 *          This command takes the exponential of each data point.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  UOM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    HDR:     DEPMIN, DEPMAX, DEPMEN
	 *    MEM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  VFLIST, VFTIME, GETFIL, EXTRMA, PUTFIL
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* CHECKING PHASE: */

	/* - Test for a non-null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Make sure each file is a time series file. */

	vftime( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */

	/* - For each file in DFL: */

	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){

		/* -- Get next file from the memory manager.
		 *   (Header is moved into common blocks CMHDR and KMHDR.) */
    if(!(s = sacget(jdfl-1, TRUE, nerr))) {
      goto L_8888;
    }
		//getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

		/* -- Take the exponential of each data point. */
		for( j = 0; j < s->h->npts; j++ ){
      s->y[j] = exp(s->y[j]);
    }

		/* -- Update any header fields that may have changed. */

		extrma( s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax, &s->h->depmen );


		}

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    831020:  Original version.
	 *===================================================================== */

} /* end of function */

