/** 
 * @file   mem.c
 * 
 * @brief Maximum entropy estimate 
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dbh.h"
#include "co.h"
#include "debug.h"

/** 
 *  Maximum entropy estimate by autocorrelation method.  Implements
 *  Yule-Walker method.
 *
 *  @param r
 *     Vector of correlation coefficients.
 *  @param n
 *     Number of lags used by MEM algorithm.
 *  @param nfft
 *     Size of fft for display.
 *  @param spect
 *     REAL*4 array of length FFT_SIZE in which the spectrum is returned.
 *  @param errmsg
 *     ERROR_MESSAGE  CHARACTER*130 variable containing error message if an
 *                   an error occurs, ' ' otherwise.
 *  @param errmsg_s
 *     Length of \p errmsg
 *  @param aux
 *     REAL*4 scratch array of length FFT_SIZE.
 *
 *  @author  David Harris
 *
 *  @date  December 28, 1984 Last Modified
 * 
 */
void 
mem(float  *r, 
    int     n, 
    int     nfft, 
    float  *spect, 
    char   *errmsg, 
    int     errmsg_s, 
    float  *aux) {

	int i;
        float scale;
        float *a, *reflct;
        float *A;

	float *const Aux = &aux[0] - 1;
	float *const R = &r[0] - 1;
	float *const Spect = &spect[0] - 1;
  UNUSED(errmsg_s);

        if((a = (float *)malloc(n*sizeof(float))) == NULL){
            strcpy(errmsg, "error allocating memory--mem\n");
            return;
	}

        if((reflct = (float *)malloc(n*sizeof(float))) == NULL){
            strcpy(errmsg, "error allocating memory--mem\n");
            free(a);
            return;
	}

        A = a-1;

/*
	if( n > 100 ){
		fstrncpy( errmsg, errmsg_s-1, "MEM *** Maximum order (100) exceeded ***"
		 , 40 );
		return;
		}
*/
	/*  Zero arrays
	 * */
	zero( spect, nfft );
	zero( aux, nfft );

	/*  Invoke Levinson's recursion to compute prediction filter
	 * */
	levin( r, a, reflct, n );

	/*  Compute transfer function of prediction filter
	 * */
	for( i = 1; i <= n; i++ )
	    Spect[i] = A[i];

	fft( spect, aux, nfft, -1 );

	/*  Spectral estimate is reciprocal of filter's power spectrum
	 *
	 *    Scale factor is equal to prediction error
	 * */
	scale = 0.;
	for( i = 1; i <= n; i++ )
	    scale = scale + R[i]*A[i];

	for( i = 1; i <= nfft; i++ )
	    Spect[i] = scale/(powi(Spect[i],2) + powi(Aux[i],2));

	/*  Bye
	 * */

        free(a); free(reflct);

	return;
} /* end of function */

