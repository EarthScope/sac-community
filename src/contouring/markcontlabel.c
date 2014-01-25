/** 
 * @file   markcontlabel.c
 * 
 * @brief  Mark a line segment label
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"
#include "bool.h"
#include "co.h"

#define	MPOINTS	5

void 
markcontlabel(index, ilabel, width, angle)
int index, ilabel;
double width;
float *angle;
{
	int lchange;
	int jaction[MPOINTS], jlink[MPOINTS], jloc, jlocm1, jlocm2, 
	 jlocp1, jlocp2, jpoint[MPOINTS], jrlink[MPOINTS], jtemp;
	float anglem12, anglep12, anglepm2, correction, distance, fpoint[MPOINTS][2];

	int *const Jaction = &jaction[0] - 1;
	int *const Jlink = &jlink[0] - 1;
	int *const Jpoint = &jpoint[0] - 1;
	int *const Jrlink = &jrlink[0] - 1;


	/*=====================================================================
	 * PURPOSE:  To mark a contouring line segment label.
	 *           A gap in the segment is produced width enough for label.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    index:   The index to the point where the label is to be placed. [i]
	 *   ilabel:   The index to the label storage attributes.
	 *    width:   The width of the label gap in viewport units. [r]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    angle:   The angle in radians of the label. [r]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    contouring:  MACTIONLABEL, MACTIONMOVE, MACTIONDRAW
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:  GetContPoint, PutContPoint, GetContRlink, CalcSegAngle
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    jloc:     Pointer to current (label) point.
	 *    jlocm1:   Pointer to "minus 1" point.
	 *    jlocm2:   Pointer to "minus 2" point.
	 *    jlocp1:   Pointer to "plus 1" point.
	 *    jlocp2:   Pointer to "plus 2" point.
	 *    jpoint:   Index to a point in the segment. [i]
	 *    point:    Point in the segment. [ra=2]
	 *    jlink:    Link to next (or previous) point in segment.
	 *    jaction:  Action to be performed at point. [i]
	 *=====================================================================
	 * ASSUMPTIONS:
	 * - The curve is smoothly varying in the vicinity of the label.
	 *   This is used when computing the unconnected part of the curve
	 *   that contains the label.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900423:  Original version loosely based on loclab.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900423
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Set up initial indices. */
	jlocm2 = 1;
	jlocm1 = 2;
	jloc = 3;
	jlocp1 = 4;
	jlocp2 = 5;

	/* - Get the attributes for the point to be labeled,
	 *   including the reverse link. */

	Jpoint[jloc] = index;
	getcontpoint( Jpoint[jloc], &fpoint[jloc - 1][0], &Jlink[jloc], 
	 &Jaction[jloc] );
	getcontrlink( Jpoint[jloc], &Jrlink[jloc] );

	/* - Store label point in the "minus 1" and "plus 1" locations to start
	 *   process. This is necessary in case label is very short compared
	 *   with data spacing. */

	Jpoint[jlocm1] = Jpoint[jloc];
	fpoint[jlocm1 - 1][0] = fpoint[jloc - 1][0];
	fpoint[jlocm1 - 1][1] = fpoint[jloc - 1][1];
	Jlink[jlocm1] = Jlink[jloc];
	Jaction[jlocm1] = Jaction[jloc];
	Jrlink[jlocm1] = Jrlink[jloc];

	Jpoint[jlocp1] = Jpoint[jloc];
	fpoint[jlocp1 - 1][0] = fpoint[jloc - 1][0];
	fpoint[jlocp1 - 1][1] = fpoint[jloc - 1][1];
	Jlink[jlocp1] = Jlink[jloc];
	Jaction[jlocp1] = Jaction[jloc];
	Jrlink[jlocp1] = Jrlink[jloc];

	/* - Move forward and backward in segment one point at a time
	 *   until the distance is big enough for the label. */

L_1000:
	lchange = FALSE;

	if( Jlink[jlocp1] > 0 ){
		Jpoint[jlocp2] = Jlink[jlocp1];
		getcontpoint( Jpoint[jlocp2], &fpoint[jlocp2 - 1][0], &Jlink[jlocp2], 
		 &Jaction[jlocp2] );
		lchange = TRUE;
		}
	else{
		Jpoint[jlocp2] = Jpoint[jlocp1];
		fpoint[jlocp2 - 1][0] = fpoint[jlocp1 - 1][0];
		fpoint[jlocp2 - 1][1] = fpoint[jlocp1 - 1][1];
		Jlink[jlocp2] = Jlink[jlocp1];
		Jaction[jlocp2] = Jaction[jlocp1];
		Jrlink[jlocp2] = Jrlink[jlocp1];
		}

	if( Jrlink[jlocm1] > 0 ){
		Jpoint[jlocm2] = Jrlink[jlocm1];
		getcontpoint( Jpoint[jlocm2], &fpoint[jlocm2 - 1][0], &Jlink[jlocm2], 
		 &Jaction[jlocm2] );
		getcontrlink( Jpoint[jlocm2], &Jrlink[jlocm2] );
		lchange = TRUE;
		}
	else{
		Jpoint[jlocm2] = Jpoint[jlocm1];
		fpoint[jlocm2 - 1][0] = fpoint[jlocm1 - 1][0];
		fpoint[jlocm2 - 1][1] = fpoint[jlocm1 - 1][1];
		Jlink[jlocm2] = Jlink[jlocm1];
		Jaction[jlocm2] = Jaction[jlocm1];
		Jrlink[jlocm2] = Jrlink[jlocm1];
		}

	distance = sqrt( powi(fpoint[jlocm2 - 1][0] - fpoint[jlocp2 - 1][0],2) + 
	 powi(fpoint[jlocm2 - 1][1] - fpoint[jlocp2 - 1][1],2) );
	if( distance < width && lchange ){
		Jaction[jlocm2] = MACTIONMOVE;
		putcontpoint( Jpoint[jlocm2], &fpoint[jlocm2 - 1][0], Jlink[jlocm2], 
		 Jaction[jlocm2] );
		Jaction[jlocp2] = MACTIONMOVE;
		putcontpoint( Jpoint[jlocp2], &fpoint[jlocp2 - 1][0], Jlink[jlocp2], 
		 Jaction[jlocp2] );
		jtemp = jlocm1;
		jlocm1 = jlocm2;
		jlocm2 = jtemp;
		jtemp = jlocp1;
		jlocp1 = jlocp2;
		jlocp2 = jtemp;
		goto L_1000;
		}
	else if( lchange ){
		correction = 0.5*(distance - width);

                if((fpoint[jlocm1 - 1][1] == fpoint[jlocm2 - 1][1]) &&
                   (fpoint[jlocm1 - 1][0] == fpoint[jlocm2 - 1][0])) anglem12 = 0.0;
                else
		    anglem12 = atan2( fpoint[jlocm1 - 1][1] - fpoint[jlocm2 - 1][1], 
		                      fpoint[jlocm1 - 1][0] - fpoint[jlocm2 - 1][0] );
		fpoint[jlocm2 - 1][0] = fpoint[jlocm2 - 1][0] + correction*cos( anglem12 );
		fpoint[jlocm2 - 1][1] = fpoint[jlocm2 - 1][1] + correction*sin( anglem12 );
		putcontpoint( Jpoint[jlocm2], &fpoint[jlocm2 - 1][0], Jlink[jlocm2], 
		 Jaction[jlocm2] );

                if(( fpoint[jlocp1 - 1][1] == fpoint[jlocp2 - 1][1]) &&
                   ( fpoint[jlocp1 - 1][0] == fpoint[jlocp2 - 1][0])) anglep12 = 0.0;
                else
		    anglep12 = atan2( fpoint[jlocp1 - 1][1] - fpoint[jlocp2 - 1][1], 
		                      fpoint[jlocp1 - 1][0] - fpoint[jlocp2 - 1][0] );
		fpoint[jlocp2 - 1][0] = fpoint[jlocp2 - 1][0] + correction*cos( anglep12 );
		fpoint[jlocp2 - 1][1] = fpoint[jlocp2 - 1][1] + correction*sin( anglep12 );
		Jaction[jlocp2] = MACTIONMOVE;
		putcontpoint( Jpoint[jlocp2], &fpoint[jlocp2 - 1][0], Jlink[jlocp2], 
		 Jaction[jlocp2] );

		}

	/* - Mark the action at the requested point to be of type LABEL.
	 *   Position of this point is also adjusted for the angle. */

        if((fpoint[jlocp2 - 1][1] == fpoint[jlocm2 - 1][1]) &&
           (fpoint[jlocp2 - 1][0] == fpoint[jlocm2 - 1][0])) anglepm2 = 0.0;
        else
	    anglepm2 = atan2( fpoint[jlocp2 - 1][1] - fpoint[jlocm2 - 1][1], 
	                      fpoint[jlocp2 - 1][0] - fpoint[jlocm2 - 1][0] );

	correction = 0.5*width;
	fpoint[jloc - 1][0] = fpoint[jlocm2 - 1][0] + correction*cos( anglepm2 );
	fpoint[jloc - 1][1] = fpoint[jlocm2 - 1][1] + correction*sin( anglepm2 );

	Jaction[jloc] = MACTIONLABEL + ilabel;
	putcontpoint( Jpoint[jloc], &fpoint[jloc - 1][0], Jlink[jloc], 
	 Jaction[jloc] );

	/* - Compute the angle between for the label. */

	calcsegangle( &fpoint[jlocm2 - 1][0], &fpoint[jlocp2 - 1][0], angle );

	return;
}

