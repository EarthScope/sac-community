
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "scm.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "co.h"
#include "dbh.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"

#define	MXFIR       7
#define	NFILTHALF   100

void /*FUNCTION*/ xstretch(nerr)
int *nerr;
{
	int jdfl, jdfl_, jold, jzero, 
	 ncoef, ndatout, ndxnew, ndxold, nlnnew, 
	 nlnold, ntused;
	float c[NFILTHALF + 1];

        float *Sacmem1, *Sacmem2;

	/*=====================================================================
	 * PURPOSE: To parse and execute the action command STRETCH.
	 *          This command stretches (upsamples) data.  An optional
	 *          interpolating FIR filter may be applied.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  SCM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:    MCPFN, MCMSG, KSUBDL, KDIRDL, KTYPEA
	 *    DFM:     NDFL
	 *    FIR:     KIDFIR
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SCM:     NSTRFC, LSTRFI
	 *    HDR:     DELTA, NPTS, E, DEPMIN, DEPMAX, DEPMEN
	 *    DFM:     NLNFIL
	 *    MEM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LCIRC, VFLIST, VFEVEN,
	 *             CRNAME, RFIR, GETFIL, EXTRMA, PUTFIL
	 *    DBH:     OVLPSV
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    NDSFILES: Number of files in a given data-set. [i]
	 *    NDSFLNUM: File number (senquental) in the current data set. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920317:  Added data-set storage, two sections of code.
	 *    910331:a)Build in filter design: it no longer assumes user has 
	 *             used DFIR (now an interactive pgm) to produce coeffs.
	 *           b)For the filtered case, remove stretching and multipli-
	 *             cation by a factor (=nstrfc) which were done before
	 *             calling ovlpsv (overlap-save filtering method).  Now the
	 *             input data is given as is, to the new filter routine.
	 *    870923:  Deleted ".saf" from aux file names.
	 *    850426:  Original version from old XSC INTER.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  
	 *===================================================================== */
	/* this routine no longer reads a filter pre-designed by FIR;
	 * it designs its own from hardwired parameters
	 *      include '../../inc/fir' */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "n": set upsampling factor. */
		if( lcirc( 2, MXFIR, &cmscm.nstrfc ) ){

			/* -- "FILTER ON|OFF": turn interpolating filter on or off. */
			}
		else if( lklog( "FILTER$",8, &cmscm.lstrfi ) ){

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

	/* CHECKING PHASE: */

	/* - Test for a non-null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Make sure each file is an evenly spaced time series file. */

	vfeven( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */

	/* - Design filter: compute coefficients. */

	if( cmscm.lstrfi ){
		lpdesign( cmscm.nstrfc, 2*NFILTHALF + 1, c, &ncoef );
		if( ncoef == 0 || ncoef != NFILTHALF ){
			fprintf( stdout, "zero or wrong num of coefs returned\n" );
			goto L_8888;
			}
		}

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
		jdfl_ = jdfl - 1;

		/* -- Get next file from the memory manager.
		 *    (Header is moved into common blocks CMHDR and KMHDR.) */
		getfil( jdfl, TRUE, &nlnold, &ndxold, &ntused, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Allocate block for output. */
		nlnnew = (nlnold - 1)*cmscm.nstrfc + 1;
		allamb( &cmmem, nlnnew, &ndxnew, nerr );
		if( *nerr != 0 )
			goto L_8888;

		if( !cmscm.lstrfi ){
			/* -- Perform stretching on data file without filtering. */
                        Sacmem1 = cmmem.sacmem[ndxnew];
                        Sacmem2 = cmmem.sacmem[ndxold];
			for( jold = ndxold; jold <= (ndxold + nlnold - 2); jold++ ){
                                *Sacmem1 = *Sacmem2;
				for( jzero = 1; jzero <= (cmscm.nstrfc - 1); jzero++ ){
                                        *(++Sacmem1) = 0.0;
					}
                                Sacmem1++;
                                Sacmem2++;
			      }
                        *Sacmem1 = *(cmmem.sacmem[ndxold]+nlnold-1);
			}
		else{

			/* -- Apply interpolating filter, 
			 *    which transfers existing data to their stretched positions,
			 *    and applies filter coefficients to all data, including inserted
			 *    zeros, to obtain filtered values for these prefiltered zeros. */

			inter( cmmem.sacmem[ndxold], nlnold, cmscm.nstrfc, c, NFILTHALF, 
			 cmmem.sacmem[ndxnew], &ndatout );
			if( ndatout != nlnnew ){
				fprintf( stdout, "wrong num of data ptd returned by filter routine\n" );
				goto L_8888;
				}

			}

		/* -- Release old memory block. */
		relamb( cmmem.sacmem, ndxold, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Update any header fields that may have changed. */
		*npts = nlnnew;
		*delta = *delta/(float)( cmscm.nstrfc );
		*e = *b + *delta*(float)( *npts - 1 );
		extrma( cmmem.sacmem[ndxnew], 1, nlnnew, depmin, depmax, depmen );

		/* -- Update file pointers. */
		Nlndta[jdfl] = nlnnew;
		cmdfm.ndxdta[jdfl_][0] = ndxnew;

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
/*-------------------------------------------------------------------------- 
 *  SUBROUTINE LPDESIGN Designs a lowpass FIR filter for interpolation.           
 *
 *    Author:  Dave Harris                                                       
 *
 *    Last Modified:  March 28, 1991                                             
 *
 *    Input arguments:                                                           
 *    ----------------                                                           
 *
 *      irate      interpolation rate                                            
 *
 *      n          total filter length (two sided), = 2*nc+1                     
 *
 *    Output arguments:                                                          
 *    -----------------                                                          
 *
 *      c          coefficient array, real array of length nc+1                  
 *
 *      nc         number of coefficients (half size)                            
 *
 * */
void /*FUNCTION*/ lpdesign(irate, n, c, nc)
int irate, n;
float c[];
int *nc;
{
	int i;
	float x;

	float *const C = &c[0] - 1;




	*nc = n/2;

	/* Evaluate filter coefficients                                                  
	 * */
	C[1] = 1.0;
	for( i = 1; i <= *nc; i++ ){
		x = (M_PI/(float)( irate ))*(float)( i );
		C[i + 1] = (sin( x )/x)*(.54 + .46*cos( M_PI*(float)( i )/(float)( *nc ) ));

		}

	/* Done                                                                          
	 * */
	return;
} /* end of function */

/*------------------------------------------------------------------------------ 
 * Subroutine INTER - Signal interpolation with FIR filter                       
 *
 * Author:  Dave Harris                                                          
 *
 * Created:  June 23, 1981                                                       
 *
 * Last Modified:  January 12, 1986                                              
 *
 * Input Arguments:                                                              
 * ----------------                                                              
 *
 * X          Real array containing original signal                              
 *
 * NX         Length of X                                                        
 *
 * IRATE      Interpolation rate                                                 
 *
 * C          Array of filter coefficients:  the first coefficient (time index 0)
 *              must be 1 in this implementation, since the original data samples
 *              are just copied to the output.  Watch out, if you are using      
 *              a Parks-McClellan filter!  Coefficient sequences obtained by     
 *              windowing the infinite impulse response of the ideal low-pass    
 *              interpolation filter are ok, since they usually satisfy the      
 *              constraint.                                                      
 *
 * NC         Half number of filter coefficients                                 
 *              full number = 2*NC + 1                                           
 *
 * Output arguments:                                                             
 * -----------------                                                             
 *
 * Y           Real array containing interpolated signal                         
 *
 * NY          Number of points desired in Y:  this will be equal    
 *               to NX + ( NX - 1 )*( IRATE - 1 )                                
 *
 *  Linkage:  ZERO                                                               
 * */
void /*FUNCTION*/ inter(x, nx, irate, c, nc, y, ny)
float x[];
int nx, irate;
float c[];
int nc;
float y[];
int *ny;
{
	int i, ic, imax, ix, iy, j;
	float t;

	float *const C = &c[0] - 1;
	float *const X = &x[0] - 1;
	float *const Y = &y[0] - 1;


    *ny = 0;

	if( irate > 1 ){

		*ny = nx + (nx - 1)*(irate - 1);
		zero( y, *ny );

		/*  The samples that require no interpolation (original data samples are         
		 *    simply copied to their respective locations in the interpolated            
		 *    signal)                                                                    
		 * */
		ix = 1;
		iy = 1;
L_1:
		;
		if( ix > nx )
			goto L_2;
		Y[iy] = X[ix];
		iy = iy + irate;
		ix = ix + 1;
		goto L_1;
L_2:
		;

		/*  Now handle the interpolated samples by stretching and filtering.  There      
		 *    is some tricky indexing here to avoid multiplying by the interstitial      
		 *    zeroes introduced by stretching the input sequence.                        
		 *
		 *                           Pointer to output sequence location                  */
		iy = 2;
		/*                           Interval counter                                     */
		ic = 1;

		/*                                       Do until output slots exhausted          */
L_3:
		;
		if( iy > *ny )
			goto L_4;

		/*                                             Guard left-hand boundary           */
		i = max( iy - nc, 1 );
		/*                                             Find non-zero sample               */
		i = i/irate;
		i = i*irate + 1;
		if( i < iy - nc ){
			/*                                             Force within filter footprint      */
			i = i + irate;
			}
		/*                                             Guard right-hand boundary          */
		imax = min( iy + nc, *ny );
		/*                                             Filter coefficient pointer         */
		j = i - iy;

		/*     Filter loop                                                               
		 * */
		t = 0.;
L_5:
		;
		if( i > imax )
			goto L_6;
		t = t + Y[i]*C[labs( j ) + 1];
		i = i + irate;
		j = j + irate;
		goto L_5;
L_6:
		;
		Y[iy] = t;

		/*     Output pointer increment                                                  
		 * */
		iy = iy + 1;
		ic = ic + 1;
		if( ic == irate ){
			iy = iy + 1;
			ic = 1;
			}

		goto L_3;
L_4:
		;

		}

	/* Bye                                                                           
	 * */
	return;
} /* end of function */
/*------------------------------------------------------------------------------  */

