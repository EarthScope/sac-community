/** 
 * @file   listcontsegs.c
 * 
 * @brief  Link line segments
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mach.h"
#include "contouring.h"
#include "bot.h"
#include "co.h"


#include "ucf.h"

void 
listcontsegs()
{
	char temptext[17];
	int jaction, jlabel, jlevel, jlink, jloc, jpoint, jsegment, 
	 jstart, jstop, jtype, nc, nerr ;
        FILE *unit;
	float angle, point[2];


	float *const Point = &point[0] - 1;


	/*=====================================================================
	 * PURPOSE: To list contouring line segments.
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    contouring:  klistname
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:   NextContSeg, GetContPoint, GetContLabel
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900424:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900424
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Set up for standard output or open requested file. */
	if( memcmp(kmcontouring.klistname,"ON",2) == 0 ){
	    unit = MUNOUT;
	}
	else{
	    znfiles( &unit, kmcontouring.klistname,MCPFN+1, "TEXT",5, &nerr );
	    if( nerr == 0 ) {
		if ( fseek ( unit , 0L , SEEK_END ) != 0 )
		    fprintf ( stdout , "fseek returned error-listcontsegs\n" ) ;
	    }
	    else
		unit = MUNOUT;
	}

	/* - Loop on each segment, printing each data point and action to
	 *   be performed at that data point. */

	jsegment = 0;

	while ( nextcontseg( &jsegment, &jlevel, &jstart, &jstop ) ){
	    fprintf( unit, "segment %d \n", jsegment );
	    getcontpoint( jstart, point, &jlink, &jaction );
L_2000:
	    if( jaction == MACTIONMOVE ){
		fprintf( unit, "  MOVE:     %g %g \n", Point[1], Point[2] );
	    }
	    else if( jaction == MACTIONDRAW ){
		fprintf( unit, "  DRAW:     %g %g \n", Point[1], Point[2] );
	    }
	    else if( jaction == MACTIONTICK ){
		fprintf( unit, "  UPTICK:   %g %g \n", Point[1], Point[2] );
	    }
	    else if( jaction == -MACTIONTICK ){
		fprintf( unit, "  DOWNTICK: %g %g \n", Point[1], Point[2] );
	    }
	    else if( jaction > MACTIONLABEL ){
		jloc = jaction - MACTIONLABEL;
		getcontlabel( jloc, &jpoint, &jtype, &angle, &jlabel );
		strcpy( temptext, kmcontouring.klabel[jlabel - 1] );
		ljust( temptext,17 );
		nc = indexb( temptext,17 );
                temptext[nc] = '\0';
		fprintf( unit, "  LABEL:    %g %g %g  \"%s \"\n", 
		 Point[1], Point[2], TODEG*angle, temptext );
	    }
	    else{
		fprintf( unit, "  UNKN:     %g %g %d %d \n", Point[1], Point[2], jaction, jlink );
	    }
	    if( jlink > 0 ){
		getcontpoint( jlink, point, &jlink, &jaction );
		goto L_2000;
	    }
	}

	/* - Close file if necessary. */

	if( unit != MUNOUT )
	    zcloses( &unit, &nerr );

	return;
}

