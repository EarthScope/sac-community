/** 
 * @file   calccontlab1.c
 * 
 * @brief  Calculate Contour label locations
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mach.h"
#include "contouring.h"
#include "bool.h"
#include "co.h"

#define	MPOINTS	3

void 
calccontlabel1()
{
	int closedseg, replacemode;
	int jaction[MPOINTS], jcur, jlabel, jlabel1, jlast, jlevel, 
	 jlink[MPOINTS], jnext, jpoint[MPOINTS], jprev, jsegment, jstart, 
	 jstatus, jstop, jtemp, jtype, numlocs;
	float accumlength, anglecur, anglesave, point[MPOINTS][2], skiplength, 
	 skiplengthend;

	int *const Jaction = &jaction[0] - 1;
	int *const Jlink = &jlink[0] - 1;
	int *const Jpoint = &jpoint[0] - 1;


	/*=====================================================================
	 * PURPOSE: First pass to calculate contouring line label locations.
	 *          This pass selects initial candidates for label locations.
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:       
	 *    contouring:  nlabellist, klabellist, MLINKCLOSED, 
	 *                 MSTATUSINCOMPLETE, MSTATUSREJECTED,
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:   NextContSeg, setlinestyle, GetContPoint, move, draw
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900821:  Shortened global variables to 15 character to keep things 
	 *             working under SunOS 3.5:
	 *                 MSEGLABELELIMINATED -> MSEGLABELELIMIN
	 *                 MSEGLABELINCOMPLETE -> MSEGLABELINCOMP
	 *    900405:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900405
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Loop on each contouring line segment: */
	jsegment = 0;
    anglesave = 0.0;
    anglecur = 0.0;
    jlabel1 = 1;    
L_1000:
	if( nextcontseg( &jsegment, &jlevel, &jstart, &jstop ) ){

		/* -- Initialize loop variables. */
		numlocs = 0;
		replacemode = FALSE;

		/* -- If labels are to be placed on this line segment: */
		if( strcmp(kmcontouring.klabel[jlevel - 1],"OFF             ") != 
		 0 ){
			jprev = 1;
			jcur = 2;
			jnext = 3;
			/* --- Get last point in segment to determine if segment is closed or not. */
			Jpoint[jprev] = jstop;
			getcontpoint( Jpoint[jprev], &point[jprev - 1][0], &Jlink[jprev], 
			 &Jaction[jprev] );
			/* --- If closed segment, start search for label at beginning and go to
			 *     end of segment. If open segment (i.e. segment begins and ends at
			 *     viewport boundary), search for label starts at minimum-label-
			 *     spacing from beginning of segment.
			 *     Search stops at minimum-label-spacing for both cases. */
			if( Jlink[jprev] == MLINKCLOSED ){
				skiplength = 0.0;
				closedseg = TRUE;
				}
			else{
				skiplength = cmcontouring.minlabelspacing;
				closedseg = FALSE;
				}
			calclastpoint( jstop, cmcontouring.minlabelspacing, &jlast );
			/* --- Get first two points in segment to to start things off. */
			Jpoint[jprev] = jstart;
			getcontpoint( Jpoint[jprev], &point[jprev - 1][0], &Jlink[jprev], 
			 &Jaction[jprev] );
			Jpoint[jcur] = Jlink[jprev];
			getcontpoint( Jpoint[jcur], &point[jcur - 1][0], &Jlink[jcur], 
			 &Jaction[jcur] );
			accumlength = 0.0;
			/* --- Loop until "last" point in line segment has been processed.
			 *     Determine and save points where (candidate) labels may to be placed. */
L_2000:
			if( Jpoint[jprev] != jlast && jlast > 0 ){
				accumlength = accumlength + sqrt( powi(point[jcur - 1][0] - 
				 point[jprev - 1][0],2) + powi(point[jcur - 1][1] - 
				 point[jprev - 1][1],2) );
				Jpoint[jnext] = Jlink[jcur];
				getcontpoint( Jpoint[jnext], &point[jnext - 1][0], 
				 &Jlink[jnext], &Jaction[jnext] );
				/* ---- Compute angle from horizontal at current point. */
				calcsegangle( &point[jprev - 1][0], &point[jnext - 1][0], 
				 &anglecur );
				/* ---- If in replace mode, look for a better location.  */
				if( replacemode ){
					if( accumlength >= skiplength ){
						replacemode = FALSE;
						skiplength = cmcontouring.minlabelspacing;
						}
					else if( fabs( anglecur ) <= fabs( anglesave ) ){ 

						jtype = MLABELCANDIDATE;
						putcontlabel( jlabel, Jpoint[jcur], jtype, 
						 anglecur, jlevel );
						anglesave = anglecur;
						}
					/* ---- If in search mode, look for an acceptable location. */
					}
				else if( (fabs( anglecur ) <= cmcontouring.desiredangle && 
				 accumlength >= skiplength) || (numlocs == 0 && accumlength >= 
				 cmcontouring.maxlabelspacing) ){
					numlocs = numlocs + 1;
					jtype = MLABELCANDIDATE;
					newcontlabel( Jpoint[jcur], jtype, anglecur, jlevel, 
					 &jlabel );
					/* ---- If first label in segment, save its index.
					 *      Also if this is a closed segment and the first label was very
					 *      close to the beginning, adjust last point in segment for a label
					 *      so that they are at least minspacinglabel apart. */
					if( numlocs == 1 ){
						jlabel1 = jlabel;
						if( closedseg && (accumlength < cmcontouring.minlabelspacing) ){
							skiplengthend = cmcontouring.minlabelspacing - 
							 accumlength;
							calclastpoint( jstop, skiplengthend, &jlast );
							}
						}
					/* ---- Zero accumulated length, and go into replacement mode. */
					accumlength = 0.0;
					skiplength = cmcontouring.deslabelspacing;
					anglesave = anglecur;
					replacemode = TRUE;
					}
				/* --- Flip indices on points and loop until no more points in segment. */
				jtemp = jprev;
				jprev = jcur;
				jcur = jnext;
				jnext = jtemp;
				goto L_2000;
				}
			}

		/* -- Set segment label status. */
		if( numlocs > 0 ){
			jstatus = MSEGLABELINCOMP;
			}
		else{
			jstatus = MSEGLABELELIMIN;
			}
		putcontseglabel( jsegment, jstatus, numlocs, jlabel1 );

		/* -- Loop until there are no more segments. */
		goto L_1000;
		}
	return;
}

