/** 
 * @file   calccontlab4.c
 * 
 * @brief  Calculate contour line labels
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gdm.h"
#include "mach.h"
#include "contouring.h"
#include "bot.h"

void 
calccontlabel4()
{
	int j, jlabel, jlevel, jloc, jpoint, 
	 jsegment, jstart, jstatus, jstop, jtype, nc, numlocs;
	float angle, textwidth;

	/*=====================================================================
	 * PURPOSE: Fourth pass to calculate contouring line label locations.
	 *          This pass computes gaps and marks segments for labels.
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    contouring: 
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:   NextContSeg, GetContSegLabel, GetContLabel, GetContPoint,
	 *           PutContPoint
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900821:  Shortened global variables to 15 characters max, to keep
	 *             things working under SunOS 3.5:
	 *                   MSEGLABELCOMPLETE -> MSEGLABELCOMPLE
	 *                   MSEGLABELINCOMPLETE -> MSEGLABELINCOMP
	 *    900418:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900418
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Loop on each contouring line segment: */
	jsegment = 0;
L_1000:
	if( nextcontseg( &jsegment, &jlevel, &jstart, &jstop ) ){

		/* -- Determine whether this segment has labels. */
		getcontseglabel( jsegment, &jstatus, &numlocs, &jloc );

		/* -- If labels are to be placed on this line segment, loop through
		 *    labels and mark the ones that have been selected. */
		if( jstatus == MSEGLABELCOMPLE || jstatus == MSEGLABELINCOMP ){
			for( j = 1; j <= numlocs; j++ ){
				getcontlabel( jloc, &jpoint, &jtype, &angle, &jlabel );
				if( jtype == MLABELSELECTED || jtype == MLABELCANDIDATE ){
					nc = indexb( (char*)kmcontouring.klabel[jlabel - 1]
					 ,17 );
					getstringsize( (char*)kmcontouring.klabel[jlabel - 1]
					 , nc, &textwidth );
					markcontlabel( jpoint, jloc, textwidth + cmcontouring.widthlabels, 
					 &angle );
					putcontlabel( jloc, jpoint, jtype, angle, jlabel );
					}
				jloc = jloc + 1;
				}
			}

		/* -- Loop until there are no more segments. */
		goto L_1000;
		}
	return;
} 

