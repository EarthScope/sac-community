/** 
 * @file   mlm.c
 * 
 * @brief  Maximum likelihood estimate
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dbh.h"
#include "ucf.h"
#include "debug.h"

/** 
 *  Subroutine to compute maximum likelihood spectral estimate
 *
 * @param r
 *    REAL*4 array containing autocorrelation coefficients.
 * @param n
 *    Number of autocorrelation lags (includes zero lag)
 *    used in computation.  Integer variable.
 * @param nfft
 *    Size of fft used for calculating displayed spectrum.
 *    Integer variable.
 * @param spect 
 *    REAL*4 array of length FFT_SIZE containing spectrum.
 * @param errmsg
 *    ERROR_MESSAGE CHARACTER*130 variable containing error message if an
 *                  error is detected, ' ' otherwise.
 * @param errmsg_s
 *    Length of \p errmsg
 * @param aux
 *    REAL*4 scratch array of length FFT_SIZE
 *
 *
 * @author  David Harris
 *
 * @date   December 28, 1984 Last Modified
 *
 */
void 
mlm(float  *r, 
    int     n, 
    int     nfft, 
    float  *spect, 
    char   *errmsg, 
    int     errmsg_s, 
    float  *aux) {

	int i, j, k, n2;
        float fn;
        float *a, *b, *c, *x;

        float *C, *X;

	float *const Spect = &spect[0] - 1;
  UNUSED(errmsg_s);

        if((a = (float *)malloc(n*sizeof(float))) == NULL){
	    strcpy(errmsg,"error allocating memory--mlm\n");
	    return;
	}
        if((b = (float *)malloc(n*sizeof(float))) == NULL){
	    strcpy(errmsg,"error allocating memory--mlm\n");
	    free(a);
	    return;
	}

        if((c = (float *)malloc(n*sizeof(float))) == NULL){
	    strcpy(errmsg,"error allocating memory--mlm\n");
	    free(a); free(b);
	    return;
	}

        if((x = (float *)malloc(n*sizeof(float))) == NULL){
	    strcpy(errmsg,"error allocating memory--mlm\n");
	    free(a); free(b); free(c);
	    return;
	}

        C = c-1;
        X = x-1;

/*
	if( n > 100 ){
		fstrncpy( errmsg, errmsg_s-1, "MLM *** Maximum order (100) exceeded ***"
		 , 40 );
		return;
		}
*/
	/*  Invert autocorrelation matrix using Simpson's sideways recursion
	 *  and set up quadratic form for spectral estimate */
	zero( x, n );

	unit( r, a, n );
	/* copy( (int*)a, (int*)c, n ); */
	copy_float(a, c, n);

	n2 = n/2;
	for( i = 1; i <= n2; i++ ){
	    if( i != 1 )
		step( r, a, b, c, n );


	    for( j = i; j <= n; j++ ){
		k = n - j + i;
		X[k] = X[k] + C[j];
	    }

	    for( j = 1; j <= i; j++ ){
		k = n - i + j;
		X[k] = X[k] + C[j];
	    }

	    /* copy( (int*)c, (int*)b, n ); */
	    copy_float(c, b, n);

	}

	if( 2*n2 < n ){

	    step( r, a, b, c, n );
	    i = n2 + 1;
	    for( j = i; j <= n; j++ ){
		k = n - j + i;
		X[k] = X[k] + C[j];
	    }
	}

	/*  Evaluate quadratic form using fft */
	zero( spect, nfft );
	Spect[1] = X[n];
	for( i = 2; i <= n; i++ ){
	    Spect[i] = X[n + 1 - i];
	    Spect[nfft - n + i] = X[i - 1];
	}

	zero( aux, nfft );
	fft( spect, aux, nfft, -1 );

	/*  Invert quadratic form and scale
	 * */
	fn = (float)( n );
	for( i = 1; i <= nfft; i++ )
	    Spect[i] = fn/Spect[i];

	/*  Bye */

        free(a); free(b); free(c); free(x);

	return;
} /* end of function */

