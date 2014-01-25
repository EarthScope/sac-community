/** 
 * @file   calccontrlink.c
 * 
 * @brief  Calculate Reverse links
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"

void 
calccontrlinks()
{
	int jaction, jlevel, jlink, jpoint, jsegment, jstart, jstop;
	float point[2];



	/*=====================================================================
	 * PURPOSE: To calculate reverse links for contouring line segments.
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:        TORAD
	 *    contouring:  indexrlinks
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:   NextContSeg, GetContPoint, PutContRlink
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

		/* -- Get first point in segment and store null reverse link. */
		jpoint = jstart;
		getcontpoint( jpoint, point, &jlink, &jaction );
		putcontrlink( jpoint, 0 );

		/* -- Store reverse link for each point and get next point. */
L_2000:
		if( jlink > 0 ){
			putcontrlink( jlink, jpoint );
			jpoint = jlink;
			getcontpoint( jpoint, (float*)&point, &jlink, &jaction );
			goto L_2000;
			}
		goto L_1000;
		}

       
	return;

} /* end of function */

