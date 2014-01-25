/** 
 * @file   step.c
 * 
 * @brief  Toeplitz Matrix Inversion
 * 
 */

#include "dbh.h"

/** 
 *  Simpson's sideways recursion for toeplitz matrix inversion.
 *
 *          Computes:   R C  =  (0,F(1),F(2),...,F(N-1))'
 *             where
 *                      R B  =  (F(1),F(2),F(3),...,F(N))'
 *             and
 *                      R A  =  (1,0,0,...,0)'
 *             and
 *                      F(.) is arbitrary
 *
 *     program solves for C --  vectors A and B and correlation
 *     matrix R are input parameters.  R is specified by the
 *     correlation coefficients R(1),...,R(N).
 *
 * @param r
 * @param a
 * @param b
 * @param c
 * @param nxb
 *
 * @author  David Harris
 *
 * @date   August 4, 1980 Last Modified
 *
 */
void 
step(float *r, 
     float *a, 
     float *b, 
     float *c, 
     int    n)
{
	int j, k;
	double alpha, beta, s1, s2;

	float *const A = &a[0] - 1;
	float *const B = &b[0] - 1;
	float *const C = &c[0] - 1;
	float *const R = &r[0] - 1;

	/*       Calculate inner products
	 * */
	s1 = 0.;
	s2 = 0.;
	for( k = 2; k <= n; k++ ){
		s1 = s1 + R[k]*B[k - 1];
		s2 = s2 + R[k]*A[n + 2 - k];
	}

	/*       Compute new vector
	 * */
	alpha = -s1 + s2*B[n]/A[1];
	beta = -B[n]/A[1];
	C[1] = alpha*A[1];
	for( j = 2; j <= n; j++ ){
		C[j] = B[j - 1] + alpha*A[j] + beta*A[n + 2 - j];
		}

	return;
}

