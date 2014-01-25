/** 
 * @file   dircor.c
 * 
 * @brief  Time domain correlation
 * 
 */

#include "dbh.h"

/** 
 *
 *  Performs time-domain correlation directly from an input sequence.
 *
 * @param data1 
 *     REAL*4 Array holding first data sequence
 * @param data2
 *     REAL*4 Array holding second data sequence.
 * @param npts
 *     Integer variable containing number of samples in data sequence.
 * @param delay
 *     Integer variable containing delay (shift) between data sequences, 
 *       if desired.  Zero if not desired.
 * @param nlags
 *     Integer variable containing number of correlation lags to compute.
 * @param c
 *     REAL*4 Array of correlation samples.
 *
 *  @author  Dave Harris
 *
 *  @date  December 6, 1984  Created
 *  @date  December 18, 1984 Last Modified
 *
 * */
void 
dircor(float *data1, 
       float *data2, 
       int    npts, 
       int    delay, 
       int    nlags, 
       double *c) {

	int i, j;
	double temp;

	for( i = 0; i < nlags ; i++ ){
	    temp = 0.0e0;
	    for( j = i + delay ; j < npts; j++ )
		temp = temp + (double)data1[j]*(double)data2[j - i - delay];
	    c[i] = temp;
	}

	return;
}

