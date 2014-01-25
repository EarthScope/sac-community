
#include <math.h>

#include "pl.h"


#include "msg.h"

void /*FUNCTION*/ logdta(array, number, lfloor, floor, output, nerr)
float array[];
int number;
int lfloor;
double floor;
float output[];
int *nerr;
{
	int j;

	float *const Array = &array[0] - 1;
	float *const Output = &output[0] - 1;


	/*=====================================================================
	 * PURPOSE: To take the base 10 logarithm of an array.
	 *          Each datum is checked for a non-positive value before taking
	 *          logarithm.  If non-positive data is found, either an error
	 *          return is made or a floor value is given to that datum.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    ARRAY:   Input array of data points.
	 *    NUMBER:  Number of data points.
	 *    LFLOOR:  Logical flag. Set to .TRUE. if a floor or
	 *             minimum value is to be put on non-positive data.
	 *    FLOOR:   Floor to be put on data if LFLOOR is .TRUE.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    OUTPUT:  Output array.  May be same as input array.
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 0902.
	 *=====================================================================
	 * MODULE/LEVEL: GAM/4
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  SETMSG
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - For each data point: */

	for( j = 1; j <= number; j++ ){
	
		/* -- Check for non-positive value. */

		if( Array[j] <= 0. ){

			/* --- If non-positive, either set output to floor value
			 *     or raise error condition. */
			if( lfloor ){
				Output[j] = log10( floor );
				}
			else{
				*nerr = 902;
				setmsg( "ERROR", *nerr );
				goto L_8888;
				}

			/* --- If positive, take base 10 logarithm. */
			}
		else{
			Output[j] = log10( Array[j] );

			}

		}

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    811223:  Original version extracted from PLDTA.
	 *===================================================================== */

} /* end of function */

