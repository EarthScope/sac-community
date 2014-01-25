/** 
 * @file   unit.c
 * 
 * @brief  Calculate first column of inverse toeplitz matrix
 * 
 */

#include <stdlib.h>

#include "dbh.h"

/** 
 *  Subroutine to calculate first column of inverse toeplitz matrix
 *
 *        Solves:   R a  =  (1,0,...,0)'
 *        where R is a Toeplitz matrix specified by the correlation
 *        coefficients R(1),...,R(N)
 *
 * @param r
 * @param a
 * @param n
 *
 *  @author  David Harris
 *
 *  @date  December 28, 1984 Last Modified
 *
 */
void 
unit(float *r, 
     float *a, 
     int    n) {

	int i, j, k, nm1;

        float *x, *X;

	double alpha, beta, s;

	float *const A = &a[0] - 1;
	float *const R = &r[0] - 1;


        x = (float *)malloc(n*sizeof(float));
        X = x-1;

	zero( a, n );
        zero( x, n);

	/*        First step of recursion
	 * */
	A[1] = 1./R[1];

	/*        Remaining steps of recursion
	 * */
	nm1 = n - 1;
	for( i = 1; i <= nm1; i++ ){
		s = 0;
		for( k = 1; k <= i; k++ ){
			s = s + R[i + 2 - k]*A[k];
			}
		beta = 1./(s - 1./s);
		alpha = -beta/s;
		X[1] = alpha*A[1];
		X[i + 1] = beta*A[1];
		if( i > 1 ){
			for( k = 2; k <= i; k++ ){
				X[k] = alpha*A[k] + beta*A[i + 2 - k];
				}
			}
		for( j = 1; j <= n; j++ ){
			A[j] = X[j];
			}
		}

        free(x);

	return;
} /* end of function */

