/** 
 * @file   alias.c
 * 
 * @brief  Alias a sequence for sparse transform calculations
 */

#include <stdio.h>
#include "dbh.h"

/** 
 * Alias a sequence for sparse transform calculation
 * 
 * @param x 
 *    Input Array
 * @param origin 
 *    Index of origin in \p x
 * @param m 
 *    Length of \p x
 * @param n 
 *    Desired Length of \p y
 * @param y 
 *    Output aliased sequence of \p x
 *
 * @return Nothing
 *
 * @author Dave B. Harris
 *
 * \note
 *    - Y_i = Y_i + X_j
 *    - Where i = 1 .. \p n - 1 => 1 .. \p n - 1 => ...
 *    - And   k = 1 .. \p m - 1
 *
 * \date 810803    Created
 * \date 071022    Documented/Reviewed
 *
 */

void 
alias(float    *x, 
      int       origin, 
      int       m, 
      int       n, 
      float    *y)
{
	int iptr, optr;

	float *const X = &x[0] - 1;
	float *const Y = &y[0] - 1;


	zero( y, n );

	iptr = 1;

	if( origin == 1 ){

		optr = 1;

		}
	else if( origin > 1 ){

		optr = n + 1 - ((origin - 1) - (origin/n)*n);

    } else if (origin < 1 || origin > m ) {
        fprintf(stderr, "alias: Origin is outside of array bounds: %d [%d,%d]\n", 
                origin, 1, m);
        return;
    }

L_1:
	;
	if( iptr > m )
		goto L_2;

	Y[optr] = Y[optr] + X[iptr];

	iptr = iptr + 1;
	optr = optr + 1;

	/*  Wrap-around */
	if( optr > n ){

		optr = optr - n;

		}

	goto L_1;
L_2:
	;

	/*  Done
	 * */
	return;
}

