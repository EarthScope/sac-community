#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "eam.h"
#include "gam.h"

void /*FUNCTION*/ pkeval(array, ndxmx, si, ndxpk, nlncda)
float array[];
int ndxmx;
double si;
int ndxpk, *nlncda;
{
	int icount, ndx;
	float diff, fdnew, fdold, rmnabs, rmncrt;

	float *const Array = &array[0] - 1;


	/*=====================================================================
	 * PURPOSE: To evaluate a valid pick.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    ARRAY:   Array containing the trace. [fa]
	 *    NDXMX:   Length of ARRAY. [i]
	 *    SI:      Sampling interval, seconds. [f]
	 *    NDXPK:   Index in ARRAY of validated arrival pick. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NLNCDA:  Length of event in samples. [i]
	 *             Set to 0 if the end of the event was not found.
	 *=====================================================================
	 * MODULE/LEVEL:  EAM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    EAM:     D8, D5, C8
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  PKFILT
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    DIFF:    First difference at current data point. [f]
	 *    FDNEW:   Current filtered data value. [f]
	 *    ICOUNT:  Count of number of samples after coda level reached. [i]
	 *    NDX:     Index of current data point being analyzed. [i]
	 *    RMNABS:  Running mean absolute value of filtered data. [f]
	 *    RMNCRT:  Critical value of RMNABS.  This is the value that
	 *             determines the end of the signal or coda. [f]
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Initialize operating parameters. */
	ndx = 1;
	diff = Array[ndx];
	fdnew = 0.;
	rmnabs = 0.;
	cmeam.i8 = cmeam.d8/si + 1;
    rmncrt = -cmeam.c8;
    icount = 0;
	/* - Main loop during evaluation phase: */

L_5000:
	if( ndx < ndxmx ){

		/* -- High pass the data to remove the mean. */

		fdold = fdnew;
		pkfilt( diff, fdold, &fdnew, &rmnabs );

		/* -- Compute coda level based upon level at pick OR
		 *    based upon a preset value. */

		if( ndx == ndxpk ){
			cmeam.i5 = cmeam.d5/si + 1;
			if( cmeam.c8 > 0. ){
				rmncrt = 2.0*cmeam.c8*rmnabs;
				}
			else{
				rmncrt = -cmeam.c8;
				}
			icount = 0;
			*nlncda = cmeam.i5;

			/* -- Coda is declared when running mean falls below coda level
			 *    and stays there for D8 seconds.
			 *    Search does not start until D5 seconds after pick.
			 *    (I5 and I8 are the times, D5 and D8 converted from seconds to counts.) */

			}
		else if( ndx > (ndxpk + cmeam.i5) ){
			*nlncda = *nlncda + 1;
			if( rmnabs < rmncrt ){
				icount = icount + 1;
				if( icount >= cmeam.i8 ){
					*nlncda = *nlncda - cmeam.i8;
					if( *nlncda <= cmeam.i5 )
						*nlncda = 0;
					goto L_8888;
					}
				}
			else{
				icount = 0;
				}
			}

		/* -- Loop back during evaluation phase. */

		ndx = ndx + 1;
		diff = Array[ndx] - Array[ndx - 1];
		goto L_5000;
		}

	*nlncda = 0;

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *     860207:  Changed names of many local variables for readability.
	 *     810204:  Fixed bug when trace ending with still finding coda.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  860207
	 *===================================================================== */

} /* end of function */

