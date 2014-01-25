/** 
 * @file   getResponse.c
 * 
 * @brief  Calcuate the frequency response of a Digital Filter
 */

#include <stdlib.h>
#include <math.h>

#include "dbh.h"
#include "amf.h"
#include "hdr.h"
#include "sam.h"


#include "ucf.h"
#include "icm.h"

/** 
 * Calcuate the frequency response of a Digital Filter: amplitude phase, and group
 *   delay, as well as the time series impulse response.  Assumed all pole filter.
 *   Writes response to file.
 * 
 * @param array 
 *    Set of coefficients upon which the responses are based.
 * @param order 
 *    Length of array \p array
 * @param gain 
 *    Gain of the signal.  The numerator of the all-pole filter
 * @param kprefix 
 *    Name of the file being processed.  Forms the prefix of the 
 *    output files.
 * @param nerr 
 *    Error return flag.
 *
 * @note
 *  Method:
 *   The impulse response (h[j]) is calculated point by point from the input
 *   coeficients (a[k]) the gain (G) and previous values of h, according to the
 *   following equation: 
 *		j < 0 ;		h[j] = 0.0
 *		j = 0 ;		h[0] = G
 *		j > 0 ;		h[j] = sum( a[k]*h[j-k] ) 
 *   Because the sum refers to values of h for j < 0, i used a pointer trick to
 *   implement the loop.  One variable called impulse is a float*.  I use calloc
 *   to allocate enough space for the requisite data points (npts) plus the
 *   number of elements of h with negative j (order) to which the summation
 *   refers.  Then i point h to impulse+order.  This allows me to use negative
 *   indexes in h[ ]. Because calloc initializes the array to zero, the above 
 *   condition is met for j < 0.
 *   As an empirical test Marv Denny produced an input file called the 
 *   recursion equation:  a low-pass filter designed such that the resultant
 *   impulse response would be identical to the input function.
 *   Testing demonstrated that the resulting impulse response was shifted
 *   in time by one data point.  The shift was having a significant impact
 *   in the calculations of the phase and the group delay.  Because of 
 *   these emperical results, the impulse response is shifted forward one data
 *   point, and it is set to zero at j = 0 (h[0] = 0).
 *
 *   Using the same fft function used in SAC's fft command, the impulse response
 *   is transformed to produce a real/imagninary response.
 *
 *   While the group delay is defined in terms of the phase, it can be found
 *   more easily from the real/imaginary response which avoids the issue of
 *   phase unwrapping.  The group delay (gd) is found from the real
 *   response (R), and the imaginary response (I), and their derivatives 
 *   with respect to frequency (f) according to the following equation:
 *		gd = / R dI  -  I dR \ * ____1____ * __1__
 *		     \   df       df /   R*R + I*I   2 * PI
 *
 *   The real/imaginary responses are converted to amplitude/phase responses
 *   with the existing function:  toamph().
 *
 * @note
 *    References:
 *     Linear Prediction:  A Tutorial Review, John Makhoul, Proc. IEEE, vol. 63
 *	   pp. 561-580 April 1975; 
 *	   correction in Proc. IEEE, vol. 64, p 285, Feb. 1976.
 *	   Also appears in:  Moder Spectrum Analysis, Donald G. Childers, IEEE,
 *	   Piscataway, NJ. 1978, pp 99-118. call number TA348C11978
 *
 *     Theory and Application of Digital Signal Processing,  
 *         Lawrence R. Rabiner & Bernard Gold, Prentice-Hall, Inc. 
 *         Englewood Cliffs, NJ.  
 *	   call number TK7868D5R321975E. esp chapter 4, p 205. 
 *
 *     Time Sequence Analysis in Geophysics, E. R. Kanasewich, The University of
 *	   Alberta Press, Edmonton, Alberta, Canada, call number QE33.2K11975
 *
 *  @copyright   1999  Regents of the University of California   
 *
 *  @author  Mike Firpo
 *           Lawrence Livermore National Laboratory
 *           L-205                                
 *           P.O. Box 808                        
 *           Livermore, CA  94550               
 *           USA                               
 *           (925) 423-4827                   
 *
 *  @date    071022 Docuemnted/Reviewed
 */
void 
getResponse(float     *array, 
	    int        order, 
	    float      gain ,
	    char      *kprefix, 
	    int       *nerr)
{
	int idx, jdx , kdx ;
	const float pi = 3.14159265;
	float userData[ 10 ] ;
	float *impulse = NULL , *h ;
	float *Sacmem, *Sacmem1 , *Sacmem2 ;
	double *Real = NULL , *Imagine = NULL , *re , *im;

	int memptr[ 4 ] , nFreq = next2 ( *npts ) ;

    for(idx = 0; idx < 4; idx++) {
        memptr[idx] = -1;
    }
	/* Allocate workspace for impulse response */
	impulse = (float *) calloc ( *npts + order , sizeof( float ) ) ;
	if ( !impulse )
	    goto L_ERROR ;
	h = impulse + order ;  /* leave first set of elements at zero */

	/* Allocate space for Impulse Response, Amplitude Response,
	   Phase Response, Group Delay
	   and Impulse Response. */
	for( idx = 0; idx < 4 ; idx++ ){
	    allamb( &cmmem, nFreq, &memptr[ idx ], nerr );
	    if( *nerr != 0 )
		goto L_ERROR;
	}

	/* Allocate workspace for real/imaginary responses. 
	   Double precision for the fourier transform function. */
        Real = (double *) calloc( nFreq , sizeof( double ) ) ;
        if ( !Real ) {
            *nerr = 301 ;
            goto L_ERROR ;
        }
        Imagine = (double *) calloc( nFreq , sizeof( double ) ) ;
        if( !Imagine ) {
            *nerr = 301 ;
            free ( Real ) ;
            Real = NULL ;
            goto L_ERROR;
        }
	re = Real ;

	/* Determine Impulse Response */
	   /* store a copy of impulse response in Real for fourier transform. */
	Sacmem = cmmem.sacmem[ memptr[ 0 ] ] ;
	*re++ = *Sacmem++ = 0.0 ;

	*Sacmem = h[ 0 ] = gain ;
	*re++ = (double)*Sacmem++ ;

	/* loop over time */
	for( jdx = 1 ; jdx < *npts - 1 ; jdx++ ) {
	    /* loop over coefficients */
	    for( kdx = 1 ; kdx <= order ; kdx++ ) {
		h[ jdx ] -= array[ kdx ] * h[ jdx - kdx ] ;
	    } /* end loop over coefficients */

	    *re++ = (double) h[ jdx ] ;
	    *Sacmem++ = h[ jdx ] ;
	} /* end loop over time */

	/* Determine Real and Imaginary responses */
	re = Real ;
	im = Imagine ;

	dcpft( re , im , nFreq , 1 , cmsam.ifwd ) ;

	re = Real ;
	im = Imagine ;
	Sacmem1 = cmmem.sacmem[ memptr[ 1 ] ] ;
	Sacmem2 = cmmem.sacmem[ memptr[ 2 ] ] ;

	for( jdx = 0; jdx < nFreq; jdx++ ){
	    (*re) *= *delta ;
	    (*im) *= *delta ;
	    *Sacmem1++ = (float) *re++ ;
	    *Sacmem2++ = (float) *im++ ;
	}

	/* Determine Group Delay */
	re = Real ;
	im = Imagine ;

	for( jdx = 1; jdx < nFreq; jdx++ ){
	    /* Fill Group Delay Array */
	    cmmem.sacmem[ memptr[ 3 ] ][ jdx ] =
	      ( ( re[ jdx ] * ( im[ jdx ] - im[ jdx - 1 ] ) -
	        im[ jdx ] * ( re[ jdx ] - re[ jdx - 1 ] ) ) /
	      ( ( re[ jdx ] * re[ jdx ] + im[ jdx ] * im[ jdx ] ) * 2 * pi ) ) 
	      * nFreq * *delta ;

		/* multiplying by nFreq and delta is equivalent to dividing by
		   delta frequency which is part of the derivative process */
	}

	/* copy second point to first point in group delay */
	   /* the derivative leaves us with one less point than we started with. */
	   /* setting the first point to the second point is a way get back the
	      original npts. */
	cmmem.sacmem[ memptr[ 3 ] ][ 0 ] = cmmem.sacmem[ memptr[ 3 ] ][ 1 ] ;

	/* convert real/imaginary data to amplitude/phase */
	toamph( cmmem.sacmem[ memptr[ 1 ] ] , cmmem.sacmem[ memptr[ 2 ] ] ,
	 nFreq, cmmem.sacmem[ memptr[ 1 ] ] , cmmem.sacmem[ memptr[ 2 ] ] ) ;

	/* set userData */
	userData[ 0 ] = 5 ;
	userData[ 1 ] = 5 ;
	userData[ 2 ] = order ;
	userData[ 3 ] = SAC_FLOAT_UNDEFINED ;
	userData[ 4 ] = SAC_FLOAT_UNDEFINED ;
	userData[ 5 ] = SAC_FLOAT_UNDEFINED ;
	userData[ 6 ] = *delta ;
	userData[ 7 ] = SAC_FLOAT_UNDEFINED ;
	userData[ 8 ] = SAC_FLOAT_UNDEFINED ;
	userData[ 9 ] = SAC_FLOAT_UNDEFINED ;

	/* Write sac files. */
	fdWhitenWrite( memptr , kprefix , userData , *npts , nFreq , nerr ) ;

L_ERROR:
	/* clean up temporary memory */
	if( Real ) 
            free( Real ) ;
	if( Imagine )
            free( Imagine ) ;
	if( impulse )
            free( impulse ) ;

        for( idx = 0; idx < 4 ; idx++ ){
            if(memptr[idx] != -1) {
                relamb( cmmem.sacmem, memptr[ idx ], nerr );
            }
        }

}


