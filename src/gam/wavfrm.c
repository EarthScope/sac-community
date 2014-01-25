#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bool.h"
#include "co.h"

void /*FUNCTION*/ wavfrm(array, nst, nlen, value, mwf, iwf, lwfok)
float array[];
int nst, nlen;
double value;
int mwf, iwf[];
int *lwfok;
{
	int j, jpairs, jst, jwf, npairs;
	float fdc, fdl, sign;

	float *const Array = &array[0] - 1;
	int *const Iwf = &iwf[0] - 1;


	/* Ind
	 *=====================================================================
	 * PURPOSE:  To measure extrema and zero-crossings of a data array.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    ARRAY:   Data array.
	 *    NST:     Index in data array to start measurement at.
	 *    NLEN:    Length of data array.
	 *    VALUE:   Data value to use in determining zero crossings.
	 *    MWF:     Maximum number of indices to return.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    IWF:     Integer array containing the indices of the data array
	 *             points found.  A zero index means no point found.
	 *    LWFOK:   Set to .TRUE. if measurement was successfully completed.
	 *=====================================================================
	 * MODULE/LEVEL:  EAM/4
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Initialize index array. */
	for( j = 1; j <= mwf; j++ ){
		Iwf[j] = 0;
		}

	/* - Check for bad input. */

	if( nst <= 0 || nst > nlen )
		goto L_6000;

	/* - Determine which direction wave is going. */

	if( Array[nst] <= value ){
		sign = 1.;
		}
	else{
		sign = -1.;
		}

	/* - Find first zero crossing by searching backward from NST. */

	jwf = 1;
	for( j = nst; j >= 1; j-- ){
		if( sign*(Array[j] - value) > 0 ){
			Iwf[jwf] = j;
			goto L_2100;
			}
		}

	/* - Outer loop find on pairs of extrema/zero crossings. */

L_2100:
	npairs = (mwf - 1)/2;
	jst = nst;
	for( jpairs = 1; jpairs <= npairs; jpairs++ ){
		fdl = Array[jst] - Array[jst - 1];

		/* - Loop to find next extrema. */

		for( j = jst + 1; j <= nlen; j++ ){
			fdc = Array[j] - Array[j - 1];
			if( (fdc*fdl) <= 0. ){
				jwf = jwf + 1;
				Iwf[jwf] = j - 1;
				goto L_3100;
				}
			fdl = fdc;
			}
		goto L_6000;

		/* - Loop to find next zero crossing. */

L_3100:
		for( j = Iwf[jwf]; j <= nlen; j++ ){
			if( sign*(Array[j] - value) > 0. ){
				jwf = jwf + 1;
				Iwf[jwf] = j - 1;
				goto L_4900;
				}
			}
		goto L_6000;
L_4900:
		jst = max( 2, Iwf[jwf] + 1 );
		sign = -sign;
		}

	/* - Check consistency of results.  The indices array
	 *   should be monotonically increasing if measurement was ok. */

L_6000:
	*lwfok = TRUE;
	if( Iwf[1] <= 0 )
		*lwfok = FALSE;
	for( j = 2; j <= mwf; j++ ){
		if( Iwf[j] < Iwf[j - 1] )
			*lwfok = FALSE;
		}

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    830803:  Improved starting point logic for first extrema.
	 *    821019:  Generalized number of measurements returned.
	 *===================================================================== */

} /* end of function */

