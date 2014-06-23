
#include "uom.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "ucf.h"
#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xint(nerr)
int *nerr;
{
	int jdfl, jy;
	float deltat, hstep, prtint, totint;

  sac *s;
	/*=====================================================================
	 * PURPOSE:  To execute the action command INTEGRATE.
	 *           This command integrates each data file.
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
	 *    HDR:     IACC, IVEL, IDISP, IUNKN, LEVEN
	 *    UOM:     LMIDPT
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    DFM:     SACMEM()
	 *    HDR:     IDEP, DEPMIN, DEPMAX, DEPMEN
	 *    UOM:     LMIDPT
	 *=====================================================================
	 * GLOBAL COUPLING:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  VFLIST, VFTIME, GETFIL, EXTRMA, PUTFIL
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    HSTEP:   Half the step size.  Used to integrate evenly spaced data.
	 *    PRTINT:  Partial integral at each step.
	 *    TOTINT:  Total integral at each data point.
	 *    DELTAT:  Delta t between two points in unevenly spaced file.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920421:  Update nlndta - required after data-set implementation.
	 *    920319:  Changed logical expression "leven.ne.0" to "leven"
	 *             for portability to IBM.
	 *    901218:  Added option of (default) trapezoidal or rectangular method.
	 *    820817:  Changed to newest set of parsing and checking functions.
	 *    810514:  Added recalculation of min, max and mean.
	 *    810120:  Changed to output message retrieval from disk.
	 *    800314:  Original version.
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */
	while ( lcmore( nerr ) ){
	    /* -- Midpoint option is the default. */
	    if( lclog2( "T#RAPEZOIDAL$",14, "R#ECTANGULAR$",14, &cmuom.ltrap ) )
	    { /* do nothing */ }

	    /* -- Bad syntax. */
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();

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

	/* - Perform requested operation on each file in DFL. */

	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
	    /* -- Get file */
    if(!(s = sacget(jdfl-1, TRUE, nerr))) {
      goto L_8888;
    }
    //getfil( jdfl, TRUE, &nlen, &nlcy, &nlcx, nerr );

	    /* -- Logic for evenly spaced data. */
	    if( s->h->leven ){
		if( cmuom.ltrap ){
		    /* --- Midpoint (trapezoidal) method.  */
		    hstep = 0.5*s->h->delta;
		    totint = 0.;
		    for( jy = 0; jy < s->h->npts-1; jy++ ){
          prtint = hstep*(s->y[jy] + s->y[jy+1]);
          totint = totint + prtint;
          s->y[jy] = totint;
		    }
		}
		else{
		    /* --- Rectangular method.  */
      s->y[0] = s->h->delta * s->y[0];
      for( jy = 1; jy < s->h->npts; jy++ ){
        s->y[jy] = s->h->delta * s->y[jy] + s->y[jy-1] ;
      }
		}
	    } /* end if( s->h->leven ) */

	    /* -- Logic for unevenly spaced data. */
	    else{
		if( cmuom.ltrap ){
		    /* --- Midpoint (trapezoidal) method.  */
		    totint = 0.;
		    for( jy = 0; jy < (s->h->npts - 1); jy++ ){
          hstep = 0.5*(s->x[jy+1] - s->x[jy]);
          prtint = hstep*(s->y[jy] + s->y[jy+1]);
          totint = totint + prtint;
          s->y[jy] = totint;
          s->x[jy] += hstep;
		    }
		}
		else{
		    /* --- Rectangular method.  */
		    for( jy = 1; jy <= s->h->npts; jy++ ){
          deltat = s->x[jy] - s->x[jy-1];
          s->y[jy] = deltat*s->y[jy] + s->y[jy-1];
		    }
		}
	    } /* end else associated with if( s->h->leven ) */

	    /* -- Change the type of the dependent variable. */
	    if( s->h->idep == IACC )
		s->h->idep = IVEL;
	    else if( s->h->idep == IVEL )
		s->h->idep = IDISP;
	    else
		s->h->idep = IUNKN;

	    /* -- If using trapezoidal method, decrease NPTS by one, set B and E. */
	    if( cmuom.ltrap ){
		s->h->npts = s->h->npts - 1;
		if( s->h->leven ){
		    s->h->b = s->h->b + 0.5*s->h->delta;
		    s->h->e = s->h->b + (float)( s->h->npts - 1 )*s->h->delta;
		}
		else{
      s->h->b = s->x[0];
      s->h->e = s->x[s->h->npts-1];
		}
	    }

	    /* -- Recalculate min, max and mean. */
	    extrma( s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax, &s->h->depmen );

	} /* end for ( jdfl ) */

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

} /* end of function */

