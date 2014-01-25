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
	int j, j1, j2;
	float aj;

	float *const Array = &array[0] - 1;

	/* - Initialize output values. */
	j = 1;
	*aminm = Array[j];
	*amaxm = Array[j];
	*amean = Array[j];

	/* - Loop through array looking for extrema. */
	j1 = incrmt + 1;
	j2 = incrmt*(number - 1) + 1;
	for( j = j1; j <= j2; j += incrmt ){
		aj = Array[j];
		*amean = *amean + aj;

                if((aj - *aminm) < 0.0 ) goto L_600;
                if((aj - *amaxm) <= 0.0) goto L_1000;
                else  goto L_800;

L_600:
		*aminm = aj;
		goto L_1000;
L_800:
		*amaxm = aj;
L_1000:
		;
		}
	/* - Compute mean value. */
	*amean = *amean/(max( number, 1 ));


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

