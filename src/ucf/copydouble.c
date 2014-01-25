/** 
 * @file   copydouble.c
 * 
 * @brief  Copy a double array
 * 
 */

#include <string.h>

#include "ucf.h"

/** 
 * Copy a double precision floating point array
 * 
 * @param source 
 *    Where to copy from
 * @param length 
 *    Length of \p source and \p sink
 * @param sink 
 *    Where to copy to
 *
 * @date   890202:  Original version.
 *
 */
void 
copydouble(double *source, 
           int     length, 
           double *sink) { 
 
        int j;

	double *const Sink = &sink[0] - 1;
	double *const Source = &source[0] - 1;

	for( j = 1; j <= length; j++ ){
          Sink[j] = Source[j];
        }
        
	return;

}

void
copy_float(float  *src,
           float  *dest,
           int     n) {
  memmove(dest, src, n * sizeof(float));
}
