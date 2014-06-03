/** 
 * @file   extrma.c
 * 
 * @brief  Calculate the extrema of an array
 * 
 */

#include "ucf.h"
#include "msg.h"
#include "co.h"

double
dsum(float *in, int n) {
    int i;
    double v;
    v = 0;
    for(i = 0; i < n; i++) {
        v = v + in[i];
    }
    return v;
}

double
kahan_sum(float *in, int n) {
    int i;
    double sum, c, t, y;
    sum = 0.0;
    c   = 0.0;
    for(i = 0; i < n; i++) {
        y = in[i] - c;
        t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    return sum;
}
float
fsum(float *in, int n) {
    int i;
    float v;
    v = 0;
    for(i = 0; i < n; i++) {
        v = v + in[i];
    }
    return v;
}

/** 
 * Calculate the extrema of an array.  The minimum, maximum, and mean
 *    value are calculated.
 * 
 * @param array 
 *    Input array
 * @param incrmt 
 *    Increment of the data array
 *    - 1 Every data point
 *    - 2 Every other data point
 * @param number 
 *    Length of \p array
 * @param aminm 
 *    Output minimum
 * @param amaxm 
 *    Output maximum
 * @param amean 
 *    Output mean value
 *
 * @date   920521:  Added test for data values outside storage range.
 *                  This should be replaced by the fortran ieee err handler.
 *                  Modified VLARGE in inc/mach to closer value for SUN SPARC1+
 * @date   830810:  Cleaned up and converted to independent subroutine.
 * @date   810000:  Original version.
 *
 */
void 
extrma(float *array, 
       int    incrmt, 
       int    number, 
       float *aminm, 
       float *amaxm, 
       float *amean)
{
	int j, k;
  double vmin, vmax;
  double sum;

	/* - Loop through array looking for extrema. */
  sum  = 0.0;
  vmin = array[0];
  vmax = array[0];
	for( j = 0; j < number; j++ ){
    k = j * incrmt;
    if(array[k] <= vmin) {
      vmin = array[k];
    }
    if(array[k] >= vmax) {
      vmax = array[k];
    }
		sum = sum + array[k];
  }
	/* - Compute mean value. */
	sum = sum/(max( number, 1 ));

  *amean = sum;
  *amaxm = vmax;
  *aminm = vmin;

	/* - Test to see if anything strange happened , like RQ command
	 *   would do with inappropriate Q or C values.
	 * */
	if( *aminm < -VLARGE || *amaxm > VLARGE ){
		setmsg( "WARNING", 0 );
		apcmsg( "Data value outside system storage bounds",41 );
		aplmsg( "Maxvalue = ",12 );
		apfmsg( *amaxm );
		apcmsg( " Minvalue = ",13 );
		apfmsg( *aminm );
		outmsg();
		clrmsg();
		}

	return;
}

