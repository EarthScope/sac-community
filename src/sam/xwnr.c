
#include <string.h>

#include <float.h>

#include "sam.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "ucf.h"
#include "msg.h"
#include "clf.h"
#include "dbh.h"
#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xwnr(nerr)
int *nerr;
{
	int jdfl, ncerr, ndx1, ndx2, nlen, nlnatw, 
	 nofatw;
	double xmax, xmin;
  char *tmp;

	/*=====================================================================
	 * PURPOSE:  To execute the action command WIENER.
	 *           This command applies a Wiener filter to data in memory.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    OMN:     MBDSYN
	 *=====================================================================
	 * GLOBAL COUPLING:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    AWTWI:   Two element array.  First value contains absolute start
	 *             time for adaptive filter window, second contains stop time.
	 *=====================================================================
	 * ASSUMPTIONS:
	 *=====================================================================
	 * LIMITATIONS:
	 *=====================================================================
	 * KNOWN ERRORS:
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

		/* -- NCOEFF n:  define number of coefficients to use in filter. */
		if( lkint( "NCOEFF$",8, &cmsam.ncwien ) )
		{ /* do nothing */ }
	
		/* -- MU v/on/off:  */
		else if ( lklogr ( "MU#$", 5, &cmsam.lmu, &cmsam.wienmu ) ) {
		} 

		/* -- EPSILON v/on/off:  define epsilon  */
    else if ( lklogr ( "EPS#ILON$", 10, &cmsam.lepsilon, &cmsam.epsilon ) ) {
    }

		/* -- WINDOW:  define a new "relative time window" for filter. */
		else if( lkrtw( "WINDOW$",8, &cmsam.lrtwwi, (char*)kmsam.krtwwi
		 ,9, cmsam.ortwwi ) )
		{ /* do nothing */ }

		/* -- Bad syntax. */
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();
		}
	} /* end while */

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
		goto L_8888;

	/* CHECKING PHASE: */

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Check to make sure all files are evenly spaced time series files. */

	vfeven( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Get the next file in DFL, moving header to CMHDR. */

		getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Call the specific subroutine to work on this file. */

		if( cmsam.lrtwwi ){
			getatw( (char*)kmsam.krtwwi,9, cmsam.ortwwi, &xmin, &xmax, 
			 &nofatw, &nlnatw, nerr );
			if( *nerr != 0 ){
				*nerr = 1608;
				setmsg( "ERROR", *nerr );
                tmp = string_list_get(datafiles, jdfl-1);
                apcmsg2(tmp, strlen(tmp)+1);
				goto L_8888;
			}
		}
		else{
			nofatw = 0;
			nlnatw = *npts;
		}

		/* -- Check limits of noise window.  maf 970401 */
		  /* see if noise window is completely outside of data window. */
		if ( cmsam.ortwwi[1] < *begin || cmsam.ortwwi[0] > *ennd ) {
		    setmsg ( "ERROR" , 1615 ) ;
		    apcmsg ( " in file number " , 17 ) ;
		    apimsg ( jdfl ) ;
		    outmsg () ;
		    clrmsg () ;
		    continue ;
		}

		  /* see if noise window over extends data window at both extremes. */
		if ( cmsam.ortwwi[0] < *begin && cmsam.ortwwi[1] > *ennd ) {
                    setmsg ( "ERROR" , 1616 ) ;
                    apcmsg ( " in file number " , 17 ) ;
                    apimsg ( jdfl ) ;
                    outmsg () ;
                    clrmsg () ;
		    continue ;
                }

		  /* see if noise window is partially outside of the data window. */
                if ( cmsam.ortwwi[0] < *begin || cmsam.ortwwi[1] > *ennd ) {
                    setmsg ( "WARNING" , 1617 ) ;
                    apcmsg ( " in file number " , 17 ) ;
                    apimsg ( jdfl ) ;
                    outmsg () ;
                    clrmsg () ;
                }
		/* -- done checking limits of noise window. maf 970401 */
		
		    
		wiener( cmmem.sacmem[ndx1], nlen, nofatw + 1, nlnatw, cmsam.ncwien, 
            cmsam.lmu, (float)cmsam.wienmu, 
            cmsam.lepsilon, (float)cmsam.epsilon, cmmem.sacmem[ndx1], &ncerr ); 

		if( ncerr > 0 ){
			setmsg( "WARNING", 1609 );
            tmp = string_list_get(datafiles, jdfl-1);
            apcmsg2(tmp, strlen(tmp)+1);
			outmsg();
		}

		/* -- Update any header fields that may have changed. */

		extrma( cmmem.sacmem[ndx1], 1, nlen, depmin, depmax, depmen );

		/* -- Reverse the steps used in getting the next file in DFL. */

		putfil( jdfl, nerr );
		if( *nerr != 0 )
			goto L_8888;

	} /* end for */

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    970401:  Checks noise window against data window.  maf
	 *    970306:  Epsilon rewritten so that it can now be turned off. maf
	 *    860304:  Change in arguments to GETATW.
	 *    820621:  Changed to newest set of parsing and checking functions.
	 *             Changed method of storing relative time windows.
	 *    820331:  Combined "parse" and "control" modules.
	 *    810205:  Original version.
	 *===================================================================== */

} /* end of function */

