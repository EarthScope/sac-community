
#include <math.h>

#include "sam.h"

void /*FUNCTION*/ torlim(am, ph, num, rl, im)
float am[], ph[];
int num;
float rl[], im[];
{
	int j;
	float arg, parg;

	float *const Am = &am[0] - 1;
	float *const Im = &im[0] - 1;
	float *const Ph = &ph[0] - 1;
	float *const Rl = &rl[0] - 1;


	/*=====================================================================
	 * PURPOSE: To convert two input arrays which contain amplitude and
	 *          phase components into two output arrays containing the 
	 *          equivalent real and imaginary components.  
	 *          The input and output arrays may be be the same.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    AM:      Amplitude array. [fa]
	 *    PH:      Phase array. [fa]
	 *    NUM:     Length of input arrays. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    RL:      Real array. [fa]
	 *    IM:      Imaginary array. [fa]
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/4
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    810000:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870211
	 *===================================================================== */
	/* PROCEDURE: */
	for( j = 1; j <= num; j++ ){
		arg = Am[j];
		parg = Ph[j];
		Rl[j] = arg*cos( parg );
		Im[j] = arg*sin( parg );
		}


       
	return;

} /* end of function */

