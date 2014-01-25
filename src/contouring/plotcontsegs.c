/** 
 * @file   plotcontsegs.c
 * 
 * @brief  Plot line Segments
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mach.h"
#include "gem.h"
#include "contouring.h"
#include "bot.h"


#include "gdm.h"

#define	MPOINTS	3

void 
plotcontsegs()
{
	char khorz[9], kvert[9];
	int icolorlist, jaction[MPOINTS], jcur, jlabel, jlevel, jlink[MPOINTS], 
	 jloc, jnext, jpoint[MPOINTS], jprev, jsegment, jstart, jstop, 
	 jtemp, jtype, nc;
	float angle, angleseg, angletick, point[MPOINTS][2], pointtick[2];

	int *const Jaction = &jaction[0] - 1;
	int *const Jlink = &jlink[0] - 1;
	int *const Jpoint = &jpoint[0] - 1;
	float *const Pointtick = &pointtick[0] - 1;


	/*=====================================================================
	 * PURPOSE: To plot contouring line segments.
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:        TORAD
	 *    contouring:  MACTIONMOVE, MACTIONDRAW, 
	 *                 zlevels, nzlevels, lines, nlines, 
	 *                 ncolorlist, kcolorlist
	 *    gem:         icline.
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:   NextContSeg, GetContPoint, move, draw, text, setcolorname,
	 *           gettextjust, settextjust, setlinestyle, settextangle
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    910301:  Changed iline to icline, to avoid same named variable in
	 *             two different common blocks. (wct)
	 *    901119:  Added call to setlinestyle to LABLES to use solid lines.
	 *    900430:  Added color capability.
	 *    900405:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900405
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Save previous text justification and set new values. */
	gettextjust( khorz,9, kvert, 9 );
	settextjust( "CENTER", "CENTER" );

	/* - Loop on each contouring line segment: */

	jsegment = 0;
L_1000:
	if( nextcontseg( &jsegment, &jlevel, &jstart, &jstop ) ){

		/* -- Determine and set linestyle. */
		if( strcmp(kmcontouring.klinemode,"ON      ") == 0 ){
			cmgem.icline = jlevel%cmcontouring.nlines;
			if( cmgem.icline == 0 )
				cmgem.icline = cmcontouring.nlines;
			setlinestyle( Lines[cmgem.icline] );
			}
		else{
			setlinestyle( 0 );
			}

		/* -- Set color if requested. */
		if( strcmp(kmcontouring.kcolormode,"ON      ") == 0 ){
			icolorlist = jlevel%cmcontouring.ncolorlist;
			if( icolorlist == 0 )
				icolorlist = cmcontouring.ncolorlist;
			setcolorname( (char*)kmcontouring.kcolorlist[icolorlist - 1]
			 ,9 );
			}

		/* -- Get first point in segment and initialize counters and attributes. */
		jprev = 1;
		jcur = 2;
		jnext = 3;
		Jpoint[jprev] = jstart;
		getcontpoint( Jpoint[jprev], &point[jprev - 1][0], &Jlink[jprev], 
		 &Jaction[jprev] );
		Jpoint[jcur] = Jpoint[jprev];
		point[jcur - 1][0] = point[jprev - 1][0];
		point[jcur - 1][1] = point[jprev - 1][1];
		Jlink[jcur] = Jlink[jprev];
		Jaction[jcur] = Jaction[jprev];


		/* -- Perform MOVE or DRAW action on current point. */
L_2000:
		if( Jaction[jcur] == MACTIONMOVE ){
			move( point[jcur - 1][0], point[jcur - 1][1] );
			}
		else if( Jaction[jcur] == MACTIONDRAW ){
			draw( point[jcur - 1][0], point[jcur - 1][1] );
			}

		/* -- Get next point in segment. */
		if( Jlink[jcur] > 0 ){
			Jpoint[jnext] = Jlink[jcur];
			getcontpoint( Jpoint[jnext], &point[jnext - 1][0], &Jlink[jnext], 
			 &Jaction[jnext] );

			/* -- Perform TICK action on current point. */
			if( labs( Jaction[jcur] ) == MACTIONTICK ){
				draw( point[jcur - 1][0], point[jcur - 1][1] );
                                if((point[jnext - 1][0] == point[jprev - 1][0]) &&
                                   (point[jnext - 1][1] == point[jprev - 1][1]))
                                          angleseg = 0.0;
                                else
				   angleseg = atan2( point[jnext - 1][0] - point[jprev - 1][0], 
				                     point[jnext - 1][1] - point[jprev - 1][1] );
				if( Jaction[jcur] < 0 ){
					angletick = angleseg - TORAD*90.0;
					}
				else{
					angletick = angleseg + TORAD*90.0;
					}
				Pointtick[1] = point[jcur - 1][0] + sin( angletick )*
				 cmcontouring.ticklength;
				Pointtick[2] = point[jcur - 1][1] + cos( angletick )*
				 cmcontouring.ticklength;
				draw( Pointtick[1], Pointtick[2] );
				move( point[jcur - 1][0], point[jcur - 1][1] );

				/* -- Perform LABEL action on current point. */
				}
			else if( Jaction[jcur] > MACTIONLABEL ){
				move( point[jcur - 1][0], point[jcur - 1][1] );
				jloc = Jaction[jcur] - MACTIONLABEL;
				getcontlabel( jloc, jpoint, &jtype, &angle, &jlabel );
				settextangle( TODEG*angle );
				nc = indexb( (char*)kmcontouring.klabel[jlabel - 1]
				 ,17 );
				setlinestyle( 1 );
				settextfont( cmgem.igtfnt );
				text( (char*)kmcontouring.klabel[jlabel - 1],17, nc );
				setlinestyle( Lines[cmgem.icline] );
				}


			/* -- Flip indices on points and loop until no more points in segment. */
			jtemp = jprev;
			jprev = jcur;
			jcur = jnext;
			jnext = jtemp;
			goto L_2000;
			}
		goto L_1000;
		}

	/* - Reset some graphics attributes. */

	setlinestyle( 1 );
	settextangle( 0.0 );
	settextjust( khorz, kvert );
	setcolorname( "DEFAULT",8 );

	return;
}

