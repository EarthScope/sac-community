
#include <stdio.h>
#include <math.h>

#include "scm.h"


#include "ucf.h"
#include "debug.h"

void /*FUNCTION*/ xpowgl(data, nlen, sr, alpha, doval, irgltp, nerr)
float data[];
int nlen;
double sr, alpha, doval;
int irgltp, *nerr;
{
	int i, ibegptr, isglw, nglitches, 
	 nsglw, nwin;
	float bd[100000], sec, sumbd, t1, value;
	static float glwin = 5.0;

	float *const Bd = &bd[0] - 1;

  UNUSED(doval);

	/*=====================================================================
	 * PURPOSE:  To "deglitch" one channel using backward difference 
	 *           threshold.  Only one point glitches are modified.
	 *           Dropouts (more than one data point with the same value)
	 *           are ignored.
	 *           The glitch is replaced with a linear interpolation of 
	 *           the two surrounding data points.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    data:    Array containing data stream. [fa]
	 *    sr:      Sample rate. [f]
	 *    nlen:    Length of one channel in multplexed data stream. [i]
	 *    alpha:   Backward difference threshold (~ 0.1)
	 *    doval:   Value data is assumed to be set at for a data dropout.
	 *=====================================================================
	 * OUTPUT ARGUMENTS: 
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  EVD/1
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  linear, fill
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    bd:      Array containing backward differences. [fa]
	 *    nsglw:   Number of samples in the analysis window used when
	 *             computing fm. [i]
	 *    glwin:   Number of seconds in window
	 **            This will be changed slightly depending on number of pts.
	 *    nwin:    Number of windows to use
	 *    nglitches: Total number of glitches found
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920420:  Changed adjustable dimension array specifier from "1" to
	 *             "*", Port to IBM turned up an indexing err msg.
	 *    920218:  Modified to go into SAC
	 *    890126:  Drastically modified by SPJ
	 *    850000:  Original version.
	 *=====================================================================
	 *
	 * PROCEDURE:
	 *
	 * */

	/* - Declarations:
	 * */

	/* - Parameters:
	 * */

	/* - Initializations:
	 * */
	*nerr = 0;

	/*  adjust window length so that all data is processed */
	sec = nlen*sr;
	nwin = sec/glwin + 1;
	nsglw = nlen/nwin;
	nglitches = 0;
	data[0] = data[1];
	/* - For each window: */
	for( i = 1; i <= nwin; i++ ){
		sumbd = 0.0;
		ibegptr = (i - 1)*nsglw;
		/*          calculate backward differences in window */
		for( isglw = 1; isglw <= nsglw; isglw++ ){
			value = fabs( data[ibegptr + isglw] );
			Bd[isglw] = value - data[ibegptr + isglw - 1];
			sumbd = sumbd + fabs( Bd[isglw] );
			}

		/*     t1 is the difference threshold for the current window */
		t1 = (sumbd/nsglw)/alpha;

		/*     examine each backward difference to see if it exceeds
		 *     the threshold.  If so, check to see if it is the beginning
		 *     or the end of a droput.  If it isn't then it is a one-point 
		 *     glitch; replace offending data point with a linearly 
		 *     interpolated value. */

		for( isglw = 1; isglw <= nsglw; isglw++ ){
			if( fabs( Bd[isglw] ) > t1 ){
				if( irgltp == 1 ){
					linear( &data[ibegptr + isglw - 1], 3, &data[ibegptr + isglw - 1] );
					}
				else if( irgltp == 2 ){
					fill( &data[ibegptr + isglw], 1, 0. );
					}
				Bd[isglw + 1] = 0.0;
				nglitches = nglitches + 1;
				}
			}

		}
	fprintf( stdout, "nglitches= %d \n", nglitches );

       
	;

	return;
} /* end of function */

