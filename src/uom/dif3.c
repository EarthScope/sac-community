
#include "uom.h"

void /*FUNCTION*/ dif3(array, number, step, output)
float array[];
int number;
double step;
float output[];
{
	int j;
	float aj, ajm1, ajp1, factor;

	float *const Array = &array[0] - 1;
	float *const Output = &output[0] - 1;


	/*=====================================================================
	 * PURPOSE:  To perform three-point (centered two-point) differentiation.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    ARRAY:   Array of data to differentiate. [f]
	 *    NUMBER:  Length of ARRAY. [i]
	 *    STEP:    Step size between samples in ARRAY. [f]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    OUTPUT:  Array of differntiated data. [f]
	 *             May be the same array as input data.
	 *             Length of output array is two less than input.
	 *=====================================================================
	 * MODULE/LEVEL:  UOM/4
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    AJ:      Current data point.
	 *    AJM1:    One before current point.
	 *    AJP1:    One after current point.
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Perform three-point (centered two-point) on interior points. */
	factor = 1./(2.*step);
	ajm1 = Array[1];
	aj = Array[2];
	for( j = 1; j <= (number - 2); j++ ){
		ajp1 = Array[j + 2];
		Output[j] = factor*(ajp1 - ajm1);
		ajm1 = aj;
		aj = ajp1;
		}

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    850122:  Deleted computation of endpoints.
	 *             Added explicit output array argument.
	 *    841220:  Fixed bug involving endpoints.
	 *    831020:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850122
	 *===================================================================== */

} /* end of function */

