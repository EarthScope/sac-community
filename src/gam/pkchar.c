/** 
 * @file   pkchar.c
 * 
 * @brief  Cursor Command
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "co.h"
#include "gam.h"
#include "debug.h"

void pkchar ( float *array, 
              int    ndxmx, 
              float si, 
              int    ndxpk, 
              char  *ktype, 
              char  *kdir, 
              char  *kqual) {
	int iqual, j, n, ndx, npeaks;
	float ampmx, backlv, diff, diffj, diffpk, fac, fdnew, fdold, fdpk, 
	 peaks[4], rbcklv, rmnabs, xon, xp1, xp2, xp3;
	static float amp[3-(0)+1]={0.,0.,0.,0.};
	static char kaqual[3-(0)+1]={'0','1','2','3'};

	float *const Array = &array[0] - 1;
	float *const Peaks = &peaks[0] - 1;
  UNUSED(si);
    memset(peaks, 0, sizeof(peaks));
	/*=====================================================================
	 * PURPOSE: To characterize the quality and sense of motion of a valid pick.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    ARRAY:   Array containing the trace.
	 *    NDXMX:   Length of ARRAY.
	 *    SI:      Sampling interval, seconds.
	 *    NDXPK:   Index of valid pick in ARRAY.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    KTYPE:   Type of arrival [char*1]
	 *             ='I' for impulsive.
	 *             ='E' for emergent.
	 *    KDIR:    Direction of first motion [char*1].
	 *             ='U' or '+' for up.
	 *             ='D' or '-' for down.
	 *             =' ' for undeterminable.
	 *    KQUAL:   Quality of pick [char*1].
	 *             ='0' to '3' for best to worst.
	 *=====================================================================
	 * MODULE/LEVEL:  EAM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  PKFILT
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    AMPMX:   Maximum amplitude store for current half cycle. [f]
	 *    BACKLV:  Measure of background noise level prior to pick. [f]
	 *    DIFF:    First difference at current data point. [f]
	 *    DIFFJ:   First difference used to calculate direction. [f]
	 *    FAC:     Scale factor used in determining sense of motion. [f]
	 *    FDNEW:   Current filtered data value. [f]
	 *    FDOLD:   Previous filtered data value. [f]
	 *    FDPK:    Filtered data value at pick. [f]
	 *    PEAKS:   Array of absolute values of first three peaks. [fa]
	 *    NDX:     Index of current data point being analyzed. [i]
	 *    NPEAKS:  Number of current entries in PEAKS. [i]
	 *    RBCKLV:  Reciprocal of BACKLV. [f]
	 *    RMNABS:  Running mean absolute value of filtered data. [f]
	 *    XON:     Scaled pick value used to estimate quality of pick. [f]
	 *    XP1:     Scaled first peak used to estimate quality of pick. [f]
	 *    XP2:     Scaled second peak used to estimate quality of pick. [f]
	 *    XP3:     Scaled third peak used to estimate quality of pick. [f]
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Compute the background noise level just before the pick. */
	ndx    = 1;
	diff   = Array[ndx];
	fdnew  = 0.;
	rmnabs = 0.;
L_2000:
	if( ndx <= ndxpk ){
		fdold = fdnew;
		pkfilt( diff, fdold, &fdnew, &rmnabs );
		ndx = ndx + 1;
		diff = Array[ndx] - Array[ndx - 1];
		goto L_2000;
		}
	backlv = 1.6*rmnabs;
	fdpk = fdnew;

	/* - Compute direction of first motion. */

	rbcklv = 1./backlv;
	diffpk = Array[ndxpk] - Array[ndxpk - 1];
	n = 1;
	j = ndxpk + 1;
L_3000:
	diffj = Array[j] - Array[j - 1];
	if( (diffpk*diffj) <= 0. ){
		if( n == 1 ){
			*kdir = ' ';
			}
		else if( diff > 0. ){
			*kdir = '+';
			if( n > 3 )
				*kdir = 'U';
			fac = fabs( Array[ndxpk] - Array[j - 1] )*rbcklv;
			if( fac > 4. )
				*kdir = 'U';
			}
		else{
			*kdir = '-';
			if( n > 3 )
				*kdir = 'D';
			fac = fabs( Array[ndxpk] - Array[j - 1] )*rbcklv;
			if( fac > 4. )
				*kdir = 'D';
			}
		}
	else{
		j = j + 1;
		n = n + 1;
		goto L_3000;
		}

	/* - Determine the absolute value of the first three peaks in the signal. */

	ndx = ndxpk + 1;
	diff = Array[ndx] - Array[ndx - 1];
	fdnew = fdpk;
	ampmx = fabs( fdnew );
	npeaks = 0;
L_4000:
	if( ndx <= ndxmx && npeaks < 3 ){
		fdold = fdnew;
		pkfilt( diff, fdold, &fdnew, &rmnabs );
                ampmx = fmax( ampmx, fabs( fdnew ) );
		if( sign( fdnew, fdold ) != fdnew ){
			npeaks = npeaks + 1;
			Peaks[npeaks] = ampmx;
			ampmx = 0.;
			}
		ndx = ndx + 1;
		diff = Array[ndx] - Array[ndx - 1];
		goto L_4000;
		}

	/* - Compute quality based on size of first 3 peaks and
	 *   first difference of signal at pick. */

	if( Peaks[1] > fabs( Array[ndxpk] ) ){
		xp1 = Peaks[1]*rbcklv;
		xp2 = Peaks[2]*rbcklv;
		xp3 = Peaks[3]*rbcklv;
		}
	else{
		xp1 = Peaks[2]*rbcklv;
		xp2 = Peaks[3]*rbcklv;
		xp3 = Peaks[4]*rbcklv;
		}
	xon = fabs( diffpk )*rbcklv;

	if( (((xp1 > 4.) && (xp2 > 6. || xp3 > 6.)) && (xon > 0.5)) && 
	 (Peaks[1] > amp[0]) ){
		iqual = 0;
		}
	else if( (((xp1 > 3.) && (xp2 > 3. || xp3 > 3.)) && (xon > 0.5)) && 
	 (Peaks[1] > amp[1]) ){
		iqual = 1;
		}
	else if( ((xp1 > 2.) && (xon > 0.5)) && (Peaks[1] > amp[2]) ){
		iqual = 2;
		}
	else{
		iqual = 3;
		}
	if( (*kdir == 'U' || *kdir == 'D') && iqual > 0 )
		iqual = iqual - 1;
	*kqual = kaqual[iqual];

	/* - Compute type of arrival based upon quality of pick. */

	if( iqual <= 1 ){
		*ktype = 'I';
		}
	else{
		*ktype = 'E';
		}

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    800502:  Original version based upon USGS coding.
	 *    801110:  Combined parts of PKR and PKPOST.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  860207
	 *===================================================================== */

} /* end of function */

