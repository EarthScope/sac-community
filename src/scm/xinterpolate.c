
#include <stdio.h>
#include <math.h>

#include "config.h"

#include "co.h"
#include "scm.h"
#include "dfm.h"
#include "hdr.h"
#include "amf.h"
#include "bool.h"

#include "errors.h"

#include "msg.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"

float geteps(float y[], int nlen, float dx);
float geteps_xy(float y[], int nlen, float x[]);
int okdf(float x[], int nlen);


void /*FUNCTION*/ xinterpolate(nerr)
int *nerr;
{
	int j, jdfl, jdfl_, nok,
	 ndxx, ndxy, newlen, newndx, nincr, nlen;
	float xnew, xstart, xstop, eps;



	/*=====================================================================
	 * PURPOSE: To parse and execute the action command INTERPOLATE.
	 *          This command interpolates data to new sampling rate.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  SCM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SCM:     DTNEW, EPS, LBREQ, BREQ, LNREQ, NREQ
	 *    MEM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP
	 *=====================================================================
	 * MODIFICATION HISTORY:
     *  20130808:  Exit if any x[j+1]-x[j] <=0 (jas)
     *  20120127:  Prints warning if new delta greater than data delta and *leven
     *  20110612:  Added Error message for interpolate
     *  20100719:  Ignored epsilon as an option.  Hardewired it to 0.0001
                       times the average slope ratio of the dY to delta (even)
                       or to dX (uneven) (jas/vt)
     *  20100707:  Fixed NPTS, total time unchanged, DELTA adjusted 
                       Changed default for dtnew to 0.0.  If not changed,
                       returns dtnew=DELTA.  If delta is called for, set lnreq
                       to be false -- do not want both npts and npts (jas/vt)
     *    100602:  Gloat to NPTS rounds off rather than truncates (bs/uri)
	 *    920227:  Added DATA-SET update logic.
	 *    871023:  Modified logic for computing interpolation window.
	 *    871012:  Added BEGIN and NPTS options.
	 *    861129:  Added ability to interpolate unevenly spaced data.
	 *    841217:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  871012
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "DELTA v":  set desired sampling interval. */
		if( lkreal( "DELTA$",7, &cmscm.dtnew ) ){
                        cmscm.lnreq = FALSE;
                        }
                        
                /* -- "BEGIN ON|OFF|v": set begin time option. */
		else if( lklogr( "BEGIN$",7, &cmscm.lbreq, &cmscm.breq ) ){

			/* -- "NPTS ON|OFF|v": set number of points option. */
			}
		else if( lklogi( "NPTS",5, &cmscm.lnreq, &cmscm.nreq ) ){

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
	 *   (2) All tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
		goto L_8888;

	/* CHECKING PHASE: */

	/* - Test for a non-null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
		jdfl_ = jdfl - 1;

		/* -- Get next file from the memory manager.
		      (Header is moved into common blocks CMHDR and KMHDR.) */
		getfil( jdfl, TRUE, &nlen, &ndxy, &ndxx, nerr );
		if( *nerr != 0 )
			goto L_8888;

        /* In the older version, dtnew = 0.025 by default.
           Now no default, so it was set to 0.0.  If dtnew
           is not explicitly set, it reverts to DELTA.  */
        if(cmscm.dtnew <= 0.0) {
            cmscm.dtnew = *delta;
        }
		/* -- Force begin time if requested. */
		if( cmscm.lbreq ){
			xstart = cmscm.breq;
			if( xstart < *b ){
                nincr = (int)lround((*b-xstart)/cmscm.dtnew)+1;
                xstart = xstart + (float)( nincr )*cmscm.dtnew;
                setmsg( "WARNING", 2008 );
                wrtmsg( stdout );
                clrmsg();
            }
            if( xstart >= *e ) {
                *nerr = ERROR_INTERPOLATE_BEGIN_TOO_LARGE;
                error(*nerr, "begin: %f e: %f ", xstart, *e);
                goto L_8888;
            }
        } else {
            xstart = *b;
        }
        
		/* -- Determine length of interpolated array, allocate block. 
                   In this version, dtnew is adjusted so that the total time
                   remains constant (jas/20100609) */
		if( cmscm.lnreq ){
			newlen = cmscm.nreq;
            xstop = *e;
            cmscm.dtnew = (xstop - xstart)/(float)( newlen - 1 );
        }
		else if( *leven ){
            newlen = (int)lround(*delta*((float)(*npts)/cmscm.dtnew));
            xstop = xstart + (float)( newlen - 1 )*cmscm.dtnew;
            if( xstop > *e ){
				newlen = newlen - 1;
				/* xstop = xstop - cmscm.dtnew; */
            }
        }
		else{
            newlen = (int)lround( (*e - xstart)/cmscm.dtnew );
            xstop = xstart + (float)( newlen - 1 )*cmscm.dtnew;
            if( xstop > *e ){
                newlen = newlen - 1;
                /* xstop = xstop - cmscm.dtnew; */
            }
        }
		
        /* Warn if dtnew greater than *delta */
    if (*leven && (float)cmscm.dtnew > *delta) {
            printf("WARNING potential for aliasing. "
                   "new delta: %f data delta: %f\n", cmscm.dtnew, *delta);
        }
        allamb( &cmmem, newlen, &newndx, nerr );
		if( *nerr != 0 )
			goto L_8888;

        /*  Calculate epsilon */
        if ( *leven ){
            eps = geteps(cmmem.sacmem[ndxy], nlen, *delta);
        }
        else{
            /* make sure no dx is less than or equal to zero*/
            *nerr = okdf(cmmem.sacmem[ndxx], nlen);
            if(*nerr) {
              goto L_8888;
            }
            eps = geteps_xy(cmmem.sacmem[ndxy], nlen, cmmem.sacmem[ndxx]);
        }
        
		/* -- Perform the specific operation on this data file. */
        if(*leven) {
          interp(cmmem.sacmem[ndxy], nlen, cmmem.sacmem[newndx], newlen,
                      *b, *e, *delta, xstart, cmscm.dtnew, eps);
        } else {
          interp2(cmmem.sacmem[ndxy], nlen, cmmem.sacmem[newndx], newlen,
                       *b, *e, cmmem.sacmem[ndxx], xstart, cmscm.dtnew, eps);
        }

		/* -- Update any header fields that may have changed. */
		*npts = newlen;
		*delta = cmscm.dtnew;
		*b = xstart;
		*e = *b + (float)( *npts - 1 )**delta;
		extrma( cmmem.sacmem[newndx], 1, newlen, depmin, depmax, depmen );

		/* -- Switch memory blocks so file now points to the new
		 *    (interpolated) array and release the orginal block. */
		cmdfm.ndxdta[jdfl_][0] = newndx;
		Nlndta[jdfl] = newlen;
		relamb( cmmem.sacmem, ndxy, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- If data was unevenly spaced, release x block 
                      and adjust header. */
		if( !*leven ){
			*leven = TRUE;
			Ncomp[jdfl] = 1;
			relamb( cmmem.sacmem, ndxx, nerr );
			if( *nerr != 0 )
				goto L_8888;
			}

		/* -- Return file to memory manager. */
		putfil( jdfl, nerr );
		if( *nerr != 0 )
			goto L_8888;

		}

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

} /* end of function */


int
okdf(float x[], int npts){
    int j;

    for( j = 0; j < (npts - 1); j++ ){
        if ((x[j+1]-x[j]) <= 0.0){
            error(ERROR_INTERPOLATE_DX_NOT_POSITIVE,
                  "j: %d x[j]: %g x[j+1] %g\n", j, x[j], x[j+1]);
            return(ERROR_INTERPOLATE_DX_NOT_POSITIVE);
        }
    }
    return SAC_OK;
}

float 
geteps(float y[], int nlen, float dx) {
    float avrat, eps;
    int j;
 
    /*  Calculate epsilon */
    avrat= 0.0;
    for( j = 0; j < (nlen - 1); j++ ){
        avrat = avrat + fabs((y[j+1]-y[j])/dx);
    }
    eps = 0.0001*avrat/(nlen-1);   
    return(eps);
} 

float 
geteps_xy(float y[], int nlen, float x[]) {
    float avrat, eps;
    int j;
    
    /*  Calculate epsilon */
    avrat= 0.0;
    for( j = 0; j <= (nlen - 1); j++ ){
        avrat = avrat + fabs((y[j+1]-y[j]) / (x[j+1]-x[j]));
    }
    eps = 0.0001*avrat/(nlen-1);    
    return(eps);
}
