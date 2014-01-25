
#include "uom.h"

void /*FUNCTION*/ dif2(array, number, step, output)
float array[];
int number;
double step;
float output[];
{
	int j;
	float factor;

	float *const Array = &array[0] - 1;
	float *const Output = &output[0] - 1;


	/*=====================================================================
	 * PURPOSE:  To perform uncentered two-point differentiation.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    ARRAY:   Array of data to differentiate. [f]
	 *    NUMBER:  Length of ARRAY. [i]
	 *    STEP:    Step size between samples in ARRAY. [f]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    OUTPUT:  Array of differentiated data. [f]
	 *             May be the same array as input data.
	 *             Length of output array is one less than input.
	 *=====================================================================
	 * MODULE/LEVEL:  UOM/4
	 *=====================================================================
	 * NOTE: Since this is not a centered differeniation, there is an
	 * implied shift in the independent variable by half the step size.
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Perform two-point differentiation on interior points. */
	factor = 1./step;
	for( j = 1; j <= (number - 1); j++ ){
		Output[j] = factor*(Array[j + 1] - Array[j]);
		}

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    850122:  Allowed output array to be different than input.
	 *    831020:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850122
	 *===================================================================== */

} /* end of function */

