/** 
 * @file   rscursor.c
 * 
 * @brief  Record Section Cursor Control
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include "sss.h"
#include "gem.h"


#include "gtm.h"
#include "bot.h"
#include "ucf.h"
#include "msg.h"
#include "gdm.h"
#include "bbs.h"

void 
rscursor ( float **limits, 
           int *action, 
           int *nerr) {

	char kvapp[9];
	char kchar;
	float atime, ddist, dtime, dvapp, dx0, dx1, dy0, dy1, xloc, xtpos, 
	 yloc, ytpos, x0, y0, x1, y1, 
	 cropTime0, cropTime1, cropDist0, cropDist1; /* maf 960716 */

	/* if the user specifies the 'C' options to crop the plot and show it
	   zoomed in, an array of four floats is returned, else NULL */
	*action = 0 ;


	/*=====================================================================
	 * PURPOSE:  To use a cursor on the record section plot for apparent 
	 *           velocities.
	 *=====================================================================
	 * RETURN VALUE:
	 *		 1 if cropping and zooming
	 *		-1 if unzooming 
	 *		 0 if quitting.
         *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *	float ** limits: if 'C' option is used to crop the plot and
	 *	                 zoom in on it, this is an array of four floats
	 *			 with the time and distance window limits,
	 *			 else NULL.  maf 960716
	 *=====================================================================
	 * MODULE/LEVEL:  GAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:  
	 *    GAM:     KGDDEF
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GAM:     LRTWXL, KRTWXL, ORTWXL
	 *=====================================================================
	 * GLOBAL COUPLING:
	 *=====================================================================
	 * GLOBAL SYSTEM INPUT:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * ASSUMPTIONS:
	 *=====================================================================
	 * LIMITATIONS:
	 *=====================================================================
	 * KNOWN ERRORS:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920727:  Orignal version from xppk.f
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* comment out plsave since its already been done 
	   in xplotrecords (pg 5-96): 
	plsave(); */

	/* - Temporarily turn on cursor graphics device only. */

	cursoron();

	kchar = 'U';

	dx0 = 0.0;
	dy0 = 0.0;

    x0 = 0.0;
    y0 = 0.0;

    cropTime0 = 0.0;
    cropDist0 = 0.0;

	/* -- Perform graphics input function. */

	xloc = cmgem.plot.xmin + 0.05*(cmgem.plot.xmax - cmgem.plot.xmin);	/* set initial ... */
	/* yloc = ypmxv - 0.5*ypdelv; */				/* ... cursor location. */
	cmgem.chht = cmgem.tsdef;
	cmgem.chwid = cmgem.txrat*cmgem.chht;
	settextsize( cmgem.chwid, cmgem.chht );
	settextangle( TEXT_HORIZONTAL );
L_4000:
	/* when a key is typed or a mouse button clicked, cursor0() passes back the 
	   current cursor location, and the char value entered. */
	cursor0( &xloc, &yloc, &kchar );
	upcase( &kchar, 1, &kchar, 1 );		/* convert kchar to uppercase */


	/* -- Kill cursor; return immediately to command level. */
	if( kchar == 'Q' || kchar == 'K' ){
	  /* comment out plhome and endframe since done in xplotrecords 
	     (pg5-96)
		plhome();
		endframe( FALSE , nerr ); */
		*action = 0 ;	/* maf 960716 */
		goto L_7777;
	} /* end if( kchar == 'Q' || kchar == 'K' ) */

	/* -- Unzoom one level, return to xplotrecords. maf 960716 */
	if ( kchar == 'O' ){
		*action = -1 ;
		goto L_7777 ;
	}


	/* - Rest of cursor responses need a valid cursor position.
	 *   For velocity, expect either a 1 or 2 as next character */

	else if( kchar == 'V' ){
	        cursor0( &xloc, &yloc, &kchar );
		if ( cmsss.lorient ) {
		  ddist = (yloc - cmgem.ympip2)/cmgem.ympip1;
		  timeadj( ddist, &atime, nerr );
		  dtime = (xloc - cmgem.xmpip2)/cmgem.xmpip1 + atime;
		}
		else {
		  ddist = (xloc - cmgem.xmpip2)/cmgem.xmpip1;
		  timeadj( ddist, &atime, nerr );
		  dtime = (yloc - cmgem.ympip2)/cmgem.ympip1 + atime;
		}

		/* - Convert xloc and yloc back into world coordinates */
		if( kchar == '1' ){
                        x0 = xloc;
                        y0 = yloc;
			dy0 = dtime;
			dx0 = ddist;
			goto L_4000;
		} /* end if( kchar == '1' ) */

		/* -- Compute apparent velocity */
		else if( kchar == '2' ){
                        x1 = xloc;
                        y1 = yloc;
			line(x0,y0,x1,y1);
		        dy1 = dtime;
			dx1 = ddist;
			dvapp = (dx1 - dx0)/(dy1 - dy0);
			fprintf( stdout, "Apparent Velocity%g \n", dvapp );
			cnvfta( dvapp, 8, 4, kvapp,9 );
			setbbv( "vapp", kvapp, nerr, 4, 8 );
			goto L_4000;
		} /* end else if( kchar == '2' ) */
	} /* end else if( kchar == 'V' ) */

	/* For cropping, expect a 1 or a 2 as next character. maf 960716 */
	else if ( kchar == 'C' ) {
	    cursor0( &xloc, &yloc, &kchar ) ;

            if ( cmsss.lorient ) {
                ddist = (yloc - cmgem.ympip2)/cmgem.ympip1;
                timeadj( ddist, &atime, nerr );
                dtime = (xloc - cmgem.xmpip2)/cmgem.xmpip1 + atime;
            }
            else {
                ddist = (xloc - cmgem.xmpip2)/cmgem.xmpip1;
                timeadj( ddist, &atime, nerr );
                dtime = (yloc - cmgem.ympip2)/cmgem.ympip1 + atime;
            }

	    if ( kchar == '1' ) {
		cropDist0 = ddist ;
		cropTime0 = dtime ;
		goto L_4000 ;
	    } /* end if ( kchar == '1' ) */

	    else if ( kchar == '2' ) {	
                cropDist1 = ddist ;
                cropTime1 = dtime ;
		*limits = ( float * ) malloc ( 4 * sizeof ( float ) ) ;
		if ( *limits == NULL ) {
		    *nerr = 301 ;
		    goto L_7777 ;
		}

		(*limits)[0] = ( cropTime0 > cropTime1 ? cropTime1 : cropTime0 ) ;
                (*limits)[1] = ( cropTime0 < cropTime1 ? cropTime1 : cropTime0 ) ;
                (*limits)[2] = ( cropDist0 > cropDist1 ? cropDist1 : cropDist0 ) ;
                (*limits)[3] = ( cropDist0 < cropDist1 ? cropDist1 : cropDist0 ) ;

		*action = 1 ;
		goto L_7777 ;
	    } /* end else if ( kchar == '2' ) */
	} /* end else if ( kchar == 'C' ) */

	/* -- Bad cursor response handled here. */
	else{
		setmsg( "OUTPUT", 1503 );
		apcmsg( &kchar,1 );
		pltmsg( &xtpos, &ytpos );
		ytpos = ytpos - cmgem.chht;
	}
	goto L_4000;


L_7777:
	/* comment out graphics reset since done in xplotrecords (pg 5-96)
	plrest(); */ 

	/* - Return to normal graphics device mode. */

	cursoroff();



       
	return ;

} /* end of function */

