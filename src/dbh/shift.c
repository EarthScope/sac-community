/** 
 * @file   shift.c
 * 
 * @brief  Shift a sequencce
 * 
 */

#include <stdlib.h>

#include "dbh.h"


#include "co.h"

/** 
 * Subroutine to shift a sequence.
 *  Circular and linear (with zero filling) shifts
 *  are implemented.
 *
 * @param x
 *    Array containing the sequence to be shifted.
 * @param n
 *    Length of the sequence.
 * @param ishft
 *    Number of samples to shift.
 *     - A negative number indicates a shift left.
 *     - A positive number indicates a shift right.
 *     - A zero indicates no shift.
 * @param type
 *    Character*8 variable indicating which type of shift.
 *     - 'C.......'    Circular shift
 *     - 'L.......'    Linear shift with zero filling.
 *
 * @param y
 *    Array containing shifted sequence.
 *    May not be the same array as X.
 * @param errmsg
 *    ERROR_MESSAGE  Character*130 variable containing error message
 *                 when error detected, ' ' when no error.
 * @param errmsg_s
 *    Length of \p errmsg
 *
 * @author  Dave Harris
 *
 * @date  January 3, 1985  Last Modified
 *
 */
void 
shift(float  *x, 
      int     n, 
      int     ishft, 
      char   *type, 
      float  *y, 
      char   *errmsg, 
      int     errmsg_s) {

	int i, is, m;

	float *const X = &x[0] - 1;
	float *const Y = &y[0] - 1;

	/*  Initializations
	 * */
        fstrncpy(errmsg, errmsg_s-1, " ", 1);

	/*  Error checking
	 *
	 *                                         Shift too large */
	if( labs( ishft ) >= n ){

                fstrncpy(errmsg, errmsg_s-1,
                  " SHIFT *** shift larger than data record *** ", 45);

		}
	else if( type[0] != 'C' && type[0] != 'L' ){

                fstrncpy(errmsg, errmsg_s-1,
                  " SHIFT *** illegal shift type *** ", 34);

		/*                                         Everything OK */
		}
	else{

		/*                                          Shift right */
		if( ishft >= 0 ){

			m = n - ishft;
			for( i = 1; i <= m; i++ ){
				Y[i + ishft] = X[i];
				}
			/*                                      Circular shift */
			if( type[0] == 'C' ){
				for( i = 1; i <= ishft; i++ ){
					Y[i] = X[n - ishft + i];
					}
				/*                                       Linear shift (zero filling) */
				}
			else{
				for( i = 1; i <= ishft; i++ ){
					Y[i] = 0.;
					}
				}

			/*                                         Shift left */
			}
		else if( ishft < 0 ){

			is = -ishft;
			m = n - is;
			for( i = 1; i <= m; i++ ){
				Y[i] = X[i + is];
				}
			/*                                       Circular shift */
			if( type[0] == 'C' ){
				for( i = 1; i <= is; i++ ){
					Y[n - is + i] = X[i];
					}
				/*                                        Linear shift (zero filling) */
				}
			else{
				for( i = 1; i <= is; i++ ){
					Y[n - is + i] = 0.;
					}
				}

			}

		}

	/*  Bye
	 * */
	return;
} /* end of function */

