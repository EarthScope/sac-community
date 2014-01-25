/** 
 * @file   fill.c
 * 
 * @brief  Fill an array 
 * 
 */

#include "ucf.h"

/** 
 * Fill an array \p array with a value \p value
 * 
 * @param array 
 *    Array to fill
 * @param number 
 *    Length of \p array
 * @param value 
 *    Value to set in array
 *
 * @date   810000:  Original version.
 *
 */
void 
fill(float  array[], 
     int    number, 
     double value)
{
	int j;

	float *const Array = &array[0] - 1;

	for( j = 1; j <= number; j++ ){
          Array[j] = value;
        }
        
	return;
}
