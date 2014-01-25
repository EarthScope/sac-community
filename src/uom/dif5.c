
#include "uom.h"

void /*FUNCTION*/ dif5(array, number, step, output)
float array[];
int number;
double step;
float output[];
{
	int j;
	float aj, ajm1, ajm2, ajp1, ajp2, dif[2], fac1, fac2;

	float *const Array = &array[0] - 1;
	float *const Dif = &dif[0] - 1;
	float *const Output = &output[0] - 1;


	/*=====================================================================
	 * PURPOSE:  To perform five-point (centered four-point) differentiation.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    ARRAY:   Array of data to differentiate. [f]
	 *    NUMBER:  Length of ARRAY. [i]
	 *    STEP:    Step size between samples in ARRAY. [f]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    OUTPUT:  Array of differentiated data. [f]
	 *             May be the same array as input data.
	 *             Length of output array is two less than input.
	 *=====================================================================
	 * MODULE/LEVEL:  UOM/4
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    AJ:      Current data point.
	 *    AJM1:    One before current.
	 *    AJM2:    Two before current.
	 *    AJP1:    One after current.
	 *    AJP2:    Two after current.
	 *    DIF:     Temporary array to hold endpoint differences.
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Do endpoints first, using two- and three-point algorithms. */
	Dif[1] = (Array[3] - Array[1])/(2.*step);
	Dif[2] = (Array[number] - Array[number - 2])/(2.*step);

	/* - Perform five-point (centered four-point) on interior points. */

	fac1 = 2./(3.*step);
	fac2 = 1./(12.*step);

	ajm2 = Array[1];
	ajm1 = Array[2];
	aj = Array[3];
	ajp1 = Array[4];

	for( j = 2; j <= (number - 3); j++ ){
		ajp2 = Array[j + 3];
		Output[j] = fac1*(ajp1 - ajm1) - fac2*(ajp2 - ajm2);
		ajm2 = ajm1;
		ajm1 = aj;
		aj = ajp1;
		ajp1 = ajp2;
		}

	/* - Store endpoints into array. */

	Output[1] = Dif[1];
	Output[number - 2] = Dif[2];

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    841220:  Fixed bug involving endpoints.
	 *    831020:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850122
	 *===================================================================== */

} /* end of function */

