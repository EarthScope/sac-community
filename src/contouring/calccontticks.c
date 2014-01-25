/** 
 * @file   calccontticks.c
 * 
 * @brief  Calculat contour link ticks
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"
#include "co.h"

#define	MPOINTS	2

void 
calccontticks()
{
	int itick, jaction[MPOINTS], jcur, jlevel, jlink[MPOINTS], 
	 jpoint[MPOINTS], jprev, jsegment, jstart, jstop, jtemp, jtick;
	float accumlength, point[MPOINTS][2], reqspacing;

	int *const Jaction = &jaction[0] - 1;
	int *const Jlink = &jlink[0] - 1;
	int *const Jpoint = &jpoint[0] - 1;


	/*=====================================================================
	 * PURPOSE: To calculate contouring line tick locations.
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    contouring:  MACTIONMOVE, MACTIONDRAW, nticklist, iticklist
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:   NextContSeg, setlinestyle, GetContPoint, move, draw
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900405:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900405
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Loop on each contouring line segment: */
	jsegment = 0;
L_1000:
	if( nextcontseg( &jsegment, &jlevel, &jstart, &jstop ) ){

		/* -- Determine whether to put tick marks on this segment. */

		itick = jlevel%cmcontouring.nticklist;
		if( itick == 0 )
			itick = cmcontouring.nticklist;
		jtick = Iticklist[itick];

		/* -- If tick marks are to be drawn on this line segment: */
		if( jtick != 0 ){
			/* --- Get first point in segment and initialize counters and attributes. */
			jprev = 1;
			jcur = 2;
			Jpoint[jprev] = jstart;
			getcontpoint( Jpoint[jprev], &point[jprev - 1][0], &Jlink[jprev], 
			 &Jaction[jprev] );
			accumlength = 0.;
			reqspacing = 0.5*cmcontouring.tickspacing;
			/* --- Loop on remainder of points in line segment.
			 *     Determine and mark points where tick marks are to be placed. */
L_2000:
			if( Jlink[jprev] > 0 ){
				Jpoint[jcur] = Jlink[jprev];
				getcontpoint( Jpoint[jcur], &point[jcur - 1][0], &Jlink[jcur], 
				 &Jaction[jcur] );
				accumlength = accumlength + sqrt( powi(point[jprev - 1][0] - 
				 point[jcur - 1][0],2) + powi(point[jprev - 1][1] - 
				 point[jcur - 1][1],2) );
				if( accumlength >= reqspacing ){
					Jaction[jcur] = jtick;
					putcontpoint( Jpoint[jcur], &point[jcur - 1][0], 
					 Jlink[jcur], Jaction[jcur] );
					accumlength = 0.;
					reqspacing = cmcontouring.tickspacing;
					}
				/* --- Flip indices on points and loop until no more points in segment. */
				jtemp = jprev;
				jprev = jcur;
				jcur = jtemp;
				goto L_2000;
				}
			}
		goto L_1000;
		}
	return;
}

