/** 
 * @file   levinD.c
 * 
 * @brief  Solve Durbin's problem, double precision
 */

#include <stdlib.h>

#include "dbh.h"

/** 
 * Solve Durbin's problem - Toeplitz normal equations, right-hand
 *    vector of autocorrelations.  The maximum problem size is 100.
 * 
 * @param r 
 *    Input vector of autocorrelations
 * @param a 
 *    Output vector of filter coefficients
 * @param reflct 
 *    Output vector of reflections coefficients, length n-1
 * @param n 
 *    Number of autocorrelations, length of \p r
 *    Includes the first coefficient which is always 1
 *    Maximum is 100
 * 
 *  @author  David Harris
 *
 *  @date 991015 Mike Firpo Made r double precision
 *  @date 960723 added casting operations to improve rounding errors
 *  @date 811010 
 *
 */
void 
levinD(double   *r, 
       float    *a, 
       float    *reflct, 
       int       n)
{
	int idx, im1, jdx;
	double rhoDenom, rhoNum, rho;
        float *temp;
        float *Temp;

	float *const A = &a[0] - 1;
	double *const R = &r[0] - 1;
	float *const Reflct = &reflct[0] - 1;

        temp = (float *)malloc(n*sizeof(float));
        Temp = temp-1;

	/*  Initialize first two coefficients
	 * */
	A[1] = 1.;
	A[2] = - R[2] / R[1] ;
	Reflct[1] = A[2];

	/*  Using Levinson's recursion, determine the rest of the coefficients.
	 *  It is assumed that the filter is of length N, including the lead
	 *  coefficient which is always one.
	 * */
	if( n >= 3 ){
	    for( idx = 3; idx <= n; idx++ ){
		im1 = idx - 1;
		rhoNum = R[idx];
		rhoDenom = R[1];

		for( jdx = 2; jdx <= im1; jdx++ ){
		    rhoNum += (double) A[jdx] * R[idx + 1 - jdx] ;
		    rhoDenom += (double) ( A[jdx] ) * R[jdx] ;
		} /* end for(jdx) */

		rho = -rhoNum/rhoDenom;
		Reflct[im1] = rho;

		for( jdx = 2; jdx <= im1; jdx++ )
		    Temp[jdx] = (double) A[jdx] +
				rho * (double) ( A[idx + 1 - jdx] ) ;

		for( jdx = 2; jdx <= im1; jdx++ )
		    A[jdx] = Temp[jdx];

		A[idx] = rho;

	    } /* end for(idx) */
	} /* end if */

        free(temp);

	return;
} /* end of function */

