
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
	int jdfl, jy, nlcx, nlcy, nlen;
	float deltat, hstep, prtint, totint;

	float *Sacmem, *Sacmem1, *Sacmem2;

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

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
	    /* -- Get file */
	    getfil( jdfl, TRUE, &nlen, &nlcy, &nlcx, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Logic for evenly spaced data. */
	    if( *leven ){
		if( cmuom.ltrap ){
		    /* --- Midpoint (trapezoidal) method.  */
		    hstep = 0.5**delta;
		    totint = 0.;
                    Sacmem = cmmem.sacmem[nlcy];
		    for( jy = 0; jy < (nlen - 1); jy++ ){
			prtint = hstep*(*Sacmem + *(Sacmem+ 1));
			totint = totint + prtint;
			*(Sacmem++) = totint;
		    }
		}
		else{
		    /* --- Rectangular method.  */
		    Sacmem = cmmem.sacmem[nlcy];
		    *Sacmem = *delta * *Sacmem ;
		    for( jy = 1, Sacmem++ ; jy < nlen; jy++, Sacmem++ ){
                        *Sacmem = *delta * *Sacmem + *(Sacmem-1) ;
		    }
		}
	    } /* end if( *leven ) */

	    /* -- Logic for unevenly spaced data. */
	    else{
		if( cmuom.ltrap ){
		    /* --- Midpoint (trapezoidal) method.  */
		    totint = 0.;
                    Sacmem1 = cmmem.sacmem[nlcy];
                    Sacmem2 = cmmem.sacmem[nlcx];
		    for( jy = 0; jy < (nlen - 1); jy++ ){
			hstep = 0.5*(*(Sacmem2+ 1) - *(Sacmem2));
			prtint = hstep*(*Sacmem1 + *(Sacmem1+ 1));
			totint = totint + prtint;
			*(Sacmem1++) = totint;
			*(Sacmem2++) += hstep;
		    }
		}
		else{
		    /* --- Rectangular method.  */
                    Sacmem1 = cmmem.sacmem[nlcx]+1;
                    Sacmem2 = cmmem.sacmem[nlcy]+1;
		    for( jy = 0; jy < nlen; jy++ ){
                        deltat = *Sacmem1 - *(Sacmem1-1);
                        *Sacmem2 = deltat**Sacmem2 + *(Sacmem2-1);
			Sacmem1++;
                        Sacmem2++;
		    }
		}
	    } /* end else associated with if( *leven ) */

	    /* -- Change the type of the dependent variable. */
	    if( *idep == *iacc )
		*idep = *ivel;
	    else if( *idep == *ivel )
		*idep = *idisp;
	    else
		*idep = *iunkn;

	    /* -- If using trapezoidal method, decrease NPTS by one, set B and E. */
	    if( cmuom.ltrap ){
		*npts = *npts - 1;
		Nlndta[jdfl] = *npts;
		if( *leven ){
		    *begin = *begin + 0.5**delta;
		    *ennd = *begin + (float)( *npts - 1 )**delta;
		}
		else{
		    *begin = *(cmmem.sacmem[nlcx]);
		    *ennd = *(cmmem.sacmem[nlcx]+*npts-1);
		}
	    }

	    /* -- Recalculate min, max and mean. */
	    extrma( cmmem.sacmem[nlcy], 1, *npts, depmin, depmax, depmen );

	    /* -- Give data file back to memory manager. */
	    putfil( jdfl, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	} /* end for ( jdfl ) */

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

} /* end of function */

