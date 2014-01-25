/** 
 * @file   rms.c
 * 
 * @brief  Compute RMS
 * 
 */

#include <math.h>

#include "dbh.h"
#include "co.h"

/** 
 *
 *  REAL*4 function to compute the rms value of an array of samples
 *
 * @param x
 *    real*4 array of samples                      
 * @param nsamps
 *    number of samples                            
 *
 * @return
 *    rms value of samples                         
 *
 * @author  Dave Harris
 *
 * @date  April 16, 1984  Created
 * @date  April 16, 1984  Last Modified
 *
 */
double 
rms(float *x, 
    int    nsamps) {

	int i;
	float rms_v;

	float *const X = &x[0] - 1;


	rms_v = 0.;
	for( i = 1; i <= nsamps; i++ ){
		rms_v = rms_v + powi(X[i],2);
	}

	rms_v = sqrt( rms_v );

	return( rms_v );
}

