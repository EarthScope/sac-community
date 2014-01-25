/** 
 * @file   linrng.c
 *
 * @brief  Check if a value is within a range
 * 
 */

#include "ucf.h"

/** 
 * Check to see if a value is within a range
 * 
 * @param value 
 *    Current value
 * @param valmin 
 *    Minimum value
 * @param valmax 
 *    MAximum value
 * 
 * @return 
 *    - TRUE if \p valmin <= \p value <= \p valmax
 *    - FALSE if not \p valmin <= \p value <= \p valmax
 *
 * @date   810000:  Original version.
 *
 */
int 
linrng(double value, 
       double valmin, 
       double valmax) {

	int linrng_v;

	linrng_v = value >= valmin && value <= valmax;

	return( linrng_v );

}

