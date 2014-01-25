
#include <math.h>

#include "sam.h"

#define PI M_PI

void /*FUNCTION*/ toamph(rl, im, num, am, ph)
float rl[], im[];
int num;
float am[], ph[];
{
	int j;
	float arg, parg, rl1;

	float *const Am = &am[0] - 1;
	float *const Im = &im[0] - 1;
	float *const Ph = &ph[0] - 1;
	float *const Rl = &rl[0] - 1;


	/*=====================================================================
	 * PURPOSE: To convert two input arrays which contain real and imaginary
	 *          components into two output arrays containing the equivalent
	 *          amplitudes and phases.  The input and output arrays may be
	 *          be the same or different arrays.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    RL:      Real array. [fa]
	 *    IM:      Imaginary array. [fa]
	 *    NUM:     Length of input arrays. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    AM:      Amplitude array. [fa]
	 *    PH:      Phase array. [fa]
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/4
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920518:  Fixed bug in computing PH(1).  Not sure why the change 
	 *             in 85, but suspect that there was a problem in using 
	 *             this routine with complex fft signals; where real values 
	 *             close to zero require using an arc tan function.
	 *    850118:  Fixed bug in computing AM(1).
	 *    810000:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870211
	 *  NOTE - THIS ROUTINE WILL NOT WORK (CONSISTENTLY) WITH COMPLEX SIGNALS.
	 *===================================================================== */
	/* PROCEDURE: */
	arg = Rl[1]*Rl[1] + Im[1]*Im[1];
	/* Save first real value because am(1) is rl(1) from calling routine. */
	rl1 = Rl[1];
	Am[1] = sqrt( arg );

	/* - BUG FIX 5/18/92 per PK et.al. */
	if( rl1 < 0 ){
		Ph[1] = PI;
	}
	else{
		Ph[1] = 0.;
	}

	for( j = 2; j <= num; j++ ){
		arg = Rl[j]*Rl[j] + Im[j]*Im[j];
		if( arg > 0. ){
			parg = atan2( Im[j], Rl[j] );
		}
		else{
			parg = 0.;
		}
		Am[j] = sqrt( arg );
		Ph[j] = parg;
	}

} /* end of function */

