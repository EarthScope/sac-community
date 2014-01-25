/** 
 * @file   winmov.c
 * 
 * @brief  Multiply two sequences
 * 
 */

#include "dbh.h"

/** 
 * Multiply two sequences
 *
 * @param data
 *    Array containing input data sequence
 * @param wlen
 *    Length of window.
 * @param w
 *    Array containing window sequence.
 * @param wdata
 *    Array containing windowed sequence
 *
 *
 * @author:  Dave Harris
 *           L-205
 *           Lawrence Livermore National Laboratory
 *           P. O. Box 808
 *           Livermore, CA  94550
 *
 * @date December 27, 1984  Created
 * @date December 27, 1984  Last Modified
 *
 */
void 
winmov(float  *data, 
       int     wlen, 
       float  *w,
       float  *wdata) {

	int i;

	for( i = 0; i < wlen; i++ ){
		wdata[i] = w[i]*data[i];
	}
}

