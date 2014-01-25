
#include "scm.h"

void /*FUNCTION*/ linear(segin, number, segout)
float segin[];
int number;
float segout[];
{
	int j;
	float step, value;

	float *const Segin = &segin[0] - 1;
	float *const Segout = &segout[0] - 1;


	/*=====================================================================
	 * PURPOSE:  To linearize a data segment.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    SEGIN:  Data array segment to be linearized. [ra]
	 *    NUMBER: Number of points in segment. [i]
	 *=====================================================================
	 *    SEGOUT: Linearized data segment. [ra]
	 *            Interior points have been replaced with a straight line
	 *            interpolation between the first and last points.
	 *            SEGOUT may be the same array as SEGIN.
	 *===================================================================== */
	step = (Segin[number] - Segin[1])/(float)( number - 1 );
	value = Segin[1];
	for( j = 1; j <= number; j++ ){
		Segout[j] = value;
		value = value + step;
		}

       
	return;

} /* end of function */

