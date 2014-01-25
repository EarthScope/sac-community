#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "dfm.h"
#include "hdr.h"
#include "amf.h"
#include "gem.h"
#include "gdm.h"
#include "gam.h"
#include "bool.h"


#include "gtm.h"
#include "pl.h"
#include "bot.h"
#include "ucf.h"
#include "msg.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"

void xplotpm(int *nerr)
{
	char kret[9], kstart[25], kstop[25], ktemp[17],
	     xlabel[MCMSG+1], ylabel[MCMSG+1];
	int lany, lframesave, lrxlim, lwait, lprint = FALSE , ltry = FALSE ;
	int ixplot, iyplot, jdfl, junk, nc, ncret, ndxx, 
	 ndxy, nlen, notused , nplot;
	float ratio, start, stop, tmax, tmin, unused, 
	 wmax, wmin, xloc, xmax, xmin, xvmax, xvmin, yloc, ymax, ymin, 
	 yvmax, yvmin;
	static char kwait[9] = "Waiting$";



	/*=====================================================================
	 * PURPOSE: To parse and execute the action command PLOTPM
	 *          This command produces a particle motion plot.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  GAM/3
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *    HDR:     B, E
	 *    GAM:     KGDDEF, LWAITR
	 *    GEM:     LXLAB, KXLAB, LYLAB, KYLAB, LTITL, KTITL
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    910608:  Added call to zgetgd when no graphics device specified.
	 *             Changed call to begindevice to begindevices. (wct)
	 *    910510:  Added lframesave variable for automatic framing test as 
	 *             to not create a new frame for each plot (wct).
	 *    870224:  Major restructuring to use new graphics library.
	 *    861022:  Now uses x and y labels if they are on.
	 *    860423:  Updated to new XSC shell format.
	 *    830124:  Modifications due to new set of parsing/checking functions.
	 *    810624:  Original version of XSC shell.
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */
	/* - Loop on each token in command: */
	while ( lcmore( nerr ) ){
	    /* if PRINT option is tried, get printer name */
	    if ( ltry ) {
		lcchar ( MAXPRNTRNAMELEN   , kmgem.kptrName ,
			 MAXPRNTRNAMELEN+1 , &notused ) ;
		terminate ( kmgem.kptrName ) ;
		if ( !lprint )
		    kmgem.kptrName[0] = '\0' ;
		ltry = FALSE ;
	    }

	    /* -- "PRINT":  print the final product */
	    else if( lckey( "PRINT#$", 8 ) ) {
		ltry = TRUE ;
		if ( cmgdm.lbegf ) {
		    setmsg ( "WARNING" , 2403 ) ;
		    outmsg () ;
		    clrmsg () ;
		}
		else {
		    lprint = TRUE ;
		}
	    }

	    /* -- Bad syntax. */
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }
	} /* end while */

	/* CHECKING PHASE: */

	/* - Test for a non-null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Make sure each file is an evenly spaced time series file. */

	vfeven( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - If no graphics device is open, try to open the default device. */

	getstatus( "ANY", &lany );
	if( !lany ){
	    zgetgd( kmgam.kgddef,9 );
	    begindevices( kmgam.kgddef,9, 1, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}

	/* EXECUTION PHASE: */

	/* - Check WAIT option.  This is on when:
	 * -- A wait request has been made.
	 * -- An active device (normally the user's terminal) is on. */

	if( cmgam.lwaitr ){
	    getstatus( "ACTIVE", &lwait );
	}
	else{
	    lwait = FALSE;
	}

	lframesave = cmgem.lframe;

	/* - For each pair of files in DFL: */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl += 2 ){
	    /* -- Get first of pair of files from the memory manager.
	     *    This will be plotted along the Y axis.
	     *    (Header is moved into common blocks CMHDR and KMHDR.) */
	    getfil( jdfl, TRUE, &nlen, &ndxy, &junk, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Determine how much of first file to plot. */
	    getxlm( &lrxlim, &tmin, &tmax );
	    if( lrxlim ){
		start = fmax( tmin, *b );
		stop = fmin( tmax, *e );
		iyplot = (int)( (start - *begin)/ *delta );
		nplot = (int)( (stop - start)/ *delta );
		extrma( cmmem.sacmem[ndxy]+iyplot,
                        1, nplot, &ymin, &ymax, &unused );
	    }
	    else{
		/* start = *b; */
		/* stop = *e; */
		iyplot = 0;
		nplot = nlen;
		ymin = *depmin;
		ymax = *depmax;
	    }

	    /* -- Set up Y axis label. */
	    if( cmgem.ylabel.on ){
		fstrncpy( ylabel, MCMSG, kmgem.kylab, strlen(kmgem.kylab));
	    }
	    else{
        lgahdr( "KSTCMP",7, ylabel,MCMSG+1 );
		if( memcmp(ylabel,"UNDEFINED",9) == 0 )
		    fstrncpy( ylabel, MCMSG, " ", 1);
	    }

	    /* -- Get second of pair of files from the memory manager.
	     *    This will be plotted along the X axis. */
	    getfil( jdfl + 1, TRUE, &nlen, &ndxx, &junk, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Determine how much of second file to plot. */
	    if( lrxlim ){
		start = fmax( tmin, *begin );
		stop = fmin( tmax, *ennd );
		ixplot = (int)( (start - *begin)/ *delta );
		extrma( cmmem.sacmem[ndxx]+ixplot,
			1, nplot, &xmin, &xmax, &unused );
	    }
	    else{
		start = *b;
		stop = *e;
		ixplot = 0;
		xmin = *depmin;
		xmax = *depmax;
	    }

	    /* -- Set up X axis label if none exists. */
	    if( cmgem.xlabel.on ){
		fstrncpy( xlabel, MCMSG, kmgem.kxlab, strlen(kmgem.kxlab));
	    }
	    else{
		lgahdr( "KSTCMP",7, xlabel,MCMSG+1 );
		if( memcmp(xlabel,"UNDEFINED",9) == 0 )
		    fstrncpy( xlabel, MCMSG, " ", 1 );
	    }

	    /* -- Set up time window display. */
	    sprintf(ktemp,"%14.6g",start);
	    ljust( ktemp,17 );
	    fstrncpy( kstart, 24, "START: ", 7);
	    fstrncpy( kstart+7, 24-7,  ktemp, strlen(ktemp));
	    sprintf(ktemp,"%14.6g",stop);
	    ljust( ktemp,17 );
	    fstrncpy( kstop, 24, "STOP:  ", 7);
	    fstrncpy( kstop+7, 24-7, ktemp, strlen(ktemp));

	    /* -- Begin new frame if necessary. 
	     *       Added framing test 5/10/91 (wct). */
	    if( lframesave ){
		beginframe( lprint , nerr );
		if( *nerr != 0 )
		    goto L_8888;
	    }

	    /* --Set viewport limits to force plot to be square. */
	    getratio( &ratio );
	    yvmin = 0.05;
	    yvmax = ratio - 0.05;
	    xvmin = 0.5*(1.0 - (yvmax - yvmin));
	    xvmax = 1.0 - xvmin;
	    setvport( xvmin, xvmax, yvmin, yvmax );

	    /* -- Set up world coordinates. */
	    wmin = fmin( xmin, ymin );
	    wmax = fmax( xmax, ymax );
	    setworld( wmin, wmax, wmin, wmax );

	    /* -- Plot this pair of files. */
	    rectangle( &xvmin, &xvmax, &yvmin, &yvmax );
	    worldpolyline( cmmem.sacmem[ndxx]+ixplot, 
                           cmmem.sacmem[ndxy]+iyplot, nplot );
	    settexttype( "SOFTWARE" );
	    xaxis( "LINEAR", "BELOW", "BOTH", xlabel,MCMSG+1 );
	    yaxis( "LINEAR", "LEFT", "BOTH", ylabel,MCMSG+1 );

	    /* -- Plot time window display. */
	    xloc = cmgem.plot.xmin + 0.01;
	    yloc = cmgem.plot.ymax - 0.01;
	    settextjust( "TOP", "LEFT" );
	    move( xloc, yloc );
	    nc = indexb( kstart,25 );
	    text( kstart,25, nc );
	    move( xloc, yloc );
	    nc = indexb( kstop,25 );
	    text( kstop,25, nc );

	    /* -- Plot title if any. */
	    if( cmgem.title.on ){
		settextjust( "BOTTOM", "CENTER" );
		move( 0.5*(xvmin + xvmax), yvmax + 0.01 );
		nc = indexb( kmgem.ktitl,145 );
		text( kmgem.ktitl,145, nc );
	    }

	    /* -- Home alphanumeric cursor and end current frame. */
	    plhome();
	    if( lframesave )
		endframe( FALSE , nerr );
            else
              flushbuffer( nerr );

	    /* -- Turn wait mode off if this is last pair and not in
		  "wait everytime" mode. */
	    if( (jdfl + 1) == (2*(cmdfm.ndfl/2)) && !cmgam.lwaite )
		lwait = FALSE;

	    /* -- If "wait mode" is on, send prompt to terminal and
		  wait for reponse. */
	    if( lwait ){
		zgpmsg( kwait,9, kret,9);
		ncret = indexb(kret,9);
		upcase( kret, ncret, kret,9 );
		if( kret[0] == 'K' )
		    goto L_8888;
		if( kret[0] == 'G' )
		    lwait = FALSE;
	    }

	}

L_8888:
	cmgem.lframe = lframesave;

	return;

} /* end of function */

