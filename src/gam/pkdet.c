#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "co.h"
#include "mach.h"
#include "eam.h"
#include "bool.h"
#include "gam.h"

void 
pkdet(float array[],
      int ndxmx,
      float si,
      int ndxst,
      int *ndxpk)
{
	int lfzc;
	int msszc, mzc, ndx, ndxend, ndxpkq, nlzc, nsszc;
	float crtinc, ecrit, eold, eref,  rbig, tmax;
        
        float rold, rdat, rlast, diff, eabs, esta, elta, edat;
        

	float *const Array = &array[0] - 1;


	/*=====================================================================
	 * PURPOSE: To detect a valid arrival pick.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    ARRAY:   Array containing the trace.
	 *    NDXMX:   Length of ARRAY.
	 *    SI:      Sampling interval, seconds
	 *    NDXST:   Index in array to start search.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NDXPK:   Index in array of a valid arrival pick.
	 *             Set to zero if no pick was found.
	 *=====================================================================
	 * MODULE/LEVEL:
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    APK:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    APK:
	 *=====================================================================
	 * GLOBAL COUPLING:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * ASSUMPTIONS:
	 *=====================================================================
	 * LIMITATIONS:
	 *=====================================================================
	 * KNOWN ERRORS:
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Initialize operating parameters and flags. */
	*ndxpk = 0;
	cmeam.i5 = cmeam.d5/si + 1;
	cmeam.i9 = cmeam.d9/si + 1;

	/* - Initialize the characteristic function and its averages. */

	ndx = ndxst;
	diff = Array[ndx];
	rdat = 0.;
	eabs = 0.;
	esta = 0.;
	elta = 0.;
	edat = 0.;
	ndxend = ndx + cmeam.i9 - 1;
	if( ndxend > ndxmx )
		ndxend = ndxmx;
L_1000:
	if( ndx <= ndxend ){
                rold = rdat;
		pkfilt( diff, rold, &rdat, &eabs );
		pkfunc( rold, rdat, &esta, &elta, &edat );
		ndx = ndx + 1;
		diff = Array[ndx] - Array[ndx - 1];
		goto L_1000;
		}
	eref = cmeam.c5*elta;

	/* - Main loop on each data point: */

	ndx = ndxend;
L_2000:
	if( ndx < ndxmx ){
		ndx = ndx + 1;

		/* -- High pass the data to remove any mean. */

		diff = Array[ndx] - Array[ndx - 1];
		eold = eref;
		rold = rdat;
		pkfilt( diff, rold, &rdat, &eabs );

		/* -- Compute characteristic function and its short and long term averages. */

		pkfunc( rold, rdat, &esta, &elta, &edat );
		/*        CALL PKSTCF(EDAT,ESTA,ELTA)
		 *
		 * -- A pick is made when the ratio of the short term average to the
		 *    long term average reaches a certain value. */

		/* -- Validate the pick: */

		eref = cmeam.c5*elta;
		if( eabs <= cmeam.c7 && esta >= eref ){

			/* --- Set up reference values and flags used in validation phase. */

			ndxpkq = ndx;
			rbig = fmax( eabs, fabs( diff/3. ) );
			tmax = fabs( rdat );
			lfzc = TRUE;
			/*          CALL PKSVCF */
			ecrit = eold;
			crtinc = eref/50000.;
			rlast = rdat;
			mzc = 1;
			nlzc = 0;
			nsszc = 0;

			/* --- For each data point during validation phase: */

L_5000:
			if( ndx < ndxmx ){

				/* ---- Filter data and compute characteristic function averages. */

				ndx = ndx + 1;
				diff = Array[ndx] - Array[ndx - 1];
				rold = rdat;
				pkfilt( diff, rold, &rdat, &eabs );
				pkfunc( rold, rdat, &esta, &elta, &edat );
				/*            CALL PKSTCF(EDAT,ESTA,ELTA) */

				/* ---- Check for large amplitude zero crossings. */
				if( fabs( rdat ) >= rbig && sign( rdat, rlast ) != rdat ){
					nlzc = nlzc + 1;
					rlast = rdat;
					}
				if( lfzc )
					tmax = fmax( tmax, fabs( rdat ) );

				/* ---- Check for zero crossings (large or small) */

				if( sign( rdat, rold ) != rdat ){
					mzc = mzc + 1;
					ecrit = ecrit + crtinc;

					/* ----- Increment or zero the successive small zero crossing counter. */

					if( esta > ecrit ){
						nsszc = 0;
						}
					else{
						nsszc = nsszc + 1;
						}

					/* ----- Adjust large zero crossing amplitude at first zero crossing. */

					if( lfzc ){
						rbig = fmax( rbig, tmax/3. );
						lfzc = FALSE;
						}
					msszc = cmeam.i3 + mzc/cmeam.i3;

					/* ----- The validation phase is over when the number of zero crossings
					 *       or the number of successive small zero crossings reach certain
					 *       values.  The pick is declared to be a valid event and the
					 *       routine terminates if the number of large zero crossings and
					 *       the event length have both reached certain values.  If the pick
					 *       does not satisfy these criteria, the algorithm continues
					 *       the top and continues its search. */

					if( mzc >= cmeam.i4 || nsszc >= msszc ){
						if( (nlzc >= cmeam.i6) && ((mzc >= cmeam.i4) || 
						 ((ndx - ndxpkq) >= cmeam.i5)) ){
							*ndxpk = ndxpkq;
							goto L_8888;
							}
						else{
							/*                  CALL PKRSCF */
							goto L_2000;
							}
						}
					}

				/* ---- Loop back during validation phase. */

				goto L_5000;
				}
			}

		/* --- Loop back during search phase. */

		goto L_2000;
		}

L_8888:
	return;

} /* end of function */

