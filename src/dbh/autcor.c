/** 
 * @file   autcor.c
 * 
 * @brief  Compute Auto-Correlation Function
 */

#include <string.h>
#include <math.h>

#include "dbh.h"
#include "spe.h"
#include "co.h"

/** 
 * Compute the Auto-Correlation Function
 * 
 * @param data 
 *    Input Array containing the data sequence
 * @param delta 
 *    Data Sampling inteval in seconds
 * @param nsamps 
 *    Length of array \p data
 * @param nwin 
 *    Requested Number of Windows
 * @param wlen 
 *    Requested Number of Samples per window.
 *    The routine will calculate the window overlap
 * @param type 
 *    Type of data analysis window to use.
 *    - <HAM>MING Hamming Window
 *    - <HAN>NING Hanning window
 *    - <C>OSINE Cosine
 *    - <R>ECTAN Rectangle
 *    - <T>RIANG Triangle
 * @param stype 
 *    Desried Scaling Type.
 *    - S for stochastic process power density scaling
 *    - T for transient energy density scaling
 * @param ac 
 *    Output Auto-Correlations
 *    - Length (2 * \p wlen) - 1
 *    - The correlation sequence is circularly rotated in the array
 *      so that the zeroth lag is in ac(0)
 * @param nfft 
 *    Output Length of Correlation Sequence
 * @param nlags 
 *    Output Number of Non-Zero correlation function samples
 * @param err 
 *    Error Message
 * @param err_s 
 *    Length of string \p err
 * @param aux 
 *    Input Scratch Space, Must be at least 5 Times \p wlen
 * @param ridge_fac 
 *    Ridge Regression Factor, Multiplied to ac(0)
 *    - Try 0.0001
 *
 * @return Nothing
 *
 * \@author  Dave Harris
 *           L-205
 *           Lawrence Livermore National Laboratory
 *           Livermore, Ca  94550
 *
 * \bug Documentation: This routine needs Documentation of Its Internals
 *
 *  \date 841226  Created and Adapted from CRSCOR
 *  \date 850103  Last Modified
 *  \date 980528  allow variable window length. maf
 *
 */

void 
autcor(float     *data, 
       double     delta, 
       int        nsamps, 
       int        nwin, 
       int        wlen, 
       char      *type, 
       char      *stype, 
       float     *ac, 
       int       *nfft, 
       int       *nlags, 
       char      *err, 
       int        err_s, 
       float     *aux, 
       float      ridge_fac)
{
	char temp[131];
	int half, i, iptr, j, k, lsamp, mxfft, nverlp, point;
	int rptr, wcntr, wptr;
	float scale, xi, xr, yi, yr;

	float *const Ac = &ac[0] - 1;
	float *const Aux = &aux[0] - 1;
	float *const Data = &data[0] - 1;

	mxfft = cmspe.firstPowerOf2 * 2;
	wptr = 1;
	rptr = 1 + wlen;
	iptr = rptr + mxfft;

	/*  Check for legal window length and compute overlap
	 * */
	*nlags = 2*wlen - 1;
	if( nwin < 1 ){
		fstrncpy( err,err_s-1, "AUTCOR  *** Too few windows ***",31);
		return;
	}
	else if( wlen < 1 || wlen > nsamps ){
		fstrncpy( err,err_s-1,"AUTCOR *** Illegal window length ***" ,36);
		return;
	}
	else{
		/* Everything OK */
		fstrncpy( err , err_s-1 , " " , 1 );

		if( nwin*wlen <= nsamps )
			nverlp = 0;
		else{
			nverlp = (nwin*wlen - nsamps)/(nwin - 1);
			if( nwin*wlen - nverlp*(nwin - 1) > nsamps ){
				nverlp = nverlp + 1;
			}
		}
		lsamp = wlen - 1;
	}

	/*  Generate window
	 * */
	for( i = 0; i <= lsamp; i++ ){
		Aux[wptr + i] = 1.;
	}
	window( &Aux[wptr], wlen, type, 1, wlen, &Aux[wptr], err ,err_s );

	/*      Normalize window
	 * */
	scale = 0.;
	for( i = 0; i <= lsamp; i++ ){
		scale = scale + powi(Aux[wptr + i],2);
	}


	if( stype[0] == 'S' ){

		scale = sqrt( delta/((float)( nwin )*scale) );

	}
	else if( stype[0] == 'T' ){

		scale = sqrt( delta*(float)( wlen )/scale );

	}

	for( i = 0; i <= lsamp; i++ ){
		Aux[wptr + i] = Aux[wptr + i]*scale;
	}

	/*  Check validity of window calculation
	 * */
	if( memcmp(err,"        ",8) != 0 ){
                fstrncpy(temp , 130 , err , strlen(err));
                fstrncpy(temp+strlen(err),130-strlen(err)," (from AUTCOR)", 14);
		fstrncpy( err, err_s-1 , temp, strlen(temp) );
		return;
	}

	/*  Find first power of two >= #LAGS
	 * */
	*nfft = 8;

	while ( *nfft < *nlags )
		( *nfft ) *= 2 ;

	half = *nfft/2;

	/*  Compute autocorrelation function
	 *
	 *
	 *    Initialize window pointer
	 * */
	point = 1;

	/*    Initialize correlation array and window counter
	 * */
	zero( ac, *nfft );
	wcntr = 0;

	/*    Compute autospectrum for each window,  then average
	 * */

	while ( wcntr < nwin ) {

	    /*    Window and load data into arrays - do two windows at a time.
	     * */
	    zero( &Aux[rptr], *nfft );
	    winmov( &Data[point], wlen, &Aux[wptr], &Aux[rptr] );
	    point = point + wlen - nverlp;
	    wcntr = wcntr + 1;

	    zero( &Aux[iptr], *nfft );
	    if( wcntr < nwin ){
		winmov( &Data[point], wlen, &Aux[wptr], &Aux[iptr] );
		point = point + wlen - nverlp;
		wcntr = wcntr + 1;
	    }

	    /*    Compute and average autospectra
	     * */
	    fft( &Aux[rptr], &Aux[iptr], *nfft, -1 );

	    /*      Special case for point at 0
	     * */
	    Ac[1] = Ac[1] + powi(Aux[rptr],2) + powi(Aux[iptr],2);

	    /*      All other points
	     * */
	    for( j = 1; j <= half; j++ ){
		k = *nfft - j;

		xr = (Aux[rptr + j] + Aux[rptr + k])*.5;
		xi = (Aux[iptr + j] - Aux[iptr + k])*.5;
		yr = (Aux[iptr + j] + Aux[iptr + k])*.5;
		yi = (Aux[rptr + k] - Aux[rptr + j])*.5;

		Ac[1 + j] = Ac[1 + j] + (powi(xr,2) + powi(xi,2)) + (powi(yr,2) + 
		 powi(yi,2));
		Ac[1 + k] = Ac[1 + j];
	    }
	} /* end while ( wcntr < nwin ) */


	/*    Inverse fft for correlation computation
	 * */
	zero( &Aux[iptr], *nfft );
	fft( &Ac[1], &Aux[iptr], *nfft, 1 );

        /* apply ridge regression factor */
/*        ac[0] *= 1.00001;  */
        ac[0] *= (1.0 + ridge_fac);

     
	/*  Bye
	 * */

	return;
}

