#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mach.h"
#include "dfm.h"
#include "hdr.h"
#include "amf.h"
#include "gem.h"
#include "gdm.h"
#include "gam.h"
#include "bool.h"


#include "pl.h"
#include "bot.h"
#include "ucf.h"
#include "msg.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"

void xp1(int *nerr)
{
        int n;
	char *kptext, kret[9];
	int l1dttm, lany, lbotaxsave, lbottcsave, lframesave, ltitlsave, 
	 ltoptcsave, lwait, lxgrdsave, lxlabsave, lxlims, lylabsave,
	 lprint = FALSE , ltry = FALSE ;
	int i, jdfl, jdfl1, jdfl2, jfr, jperfr, n1dttm[6], 
	 ncret, nfr, nlcx, nlcy, nperfr, num, notused;
	float tmax, tmaxj, tmin, tminj, toff[MDFL], ypdel, ypmxsave;

	static int lrel = FALSE;
	static int lperpl = FALSE;
	static int nperpl = 3;
	static char kwait[9] = "Waiting$";

	float *const Toff = &toff[0] - 1;


	/*=====================================================================
	 * PURPOSE:  To execute the action command P1.
	 *           This command makes a multi-trace, multi-window plot.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error return flag.  Set to 0 if no error occurred.
	 *             Potential error numbers:  1001, 1504.
	 *=====================================================================
	 * MODULE/LEVEL:  gam/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    dfm:     ndfl, sacmem
	 *    hdr:     begin, ennd, delta
	 *    gem:     lbotax, lbottc, ltopax, ltoptc, lxlab, lylab, ltitl,
	 *             lxgrd, ypmn, ypmx, chht, tsdef
	 *    gam:     kgddef
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    970908:  Modified response to ddttm.  maf 
	 *    970723:  Commented out an if statement to fix a bug which kept
	 *             p1 relative from functioning when xlim was set. maf
         *    970130:  Added arguments to dispid() to plot file number. maf
	 *    910607:  Move stmt label 8888 back to where it was.
	 *             Changed gots to goto plrest after call to plsave.
	 *             Error condition before lframesave goes to return. (wct).
	 *    910607:  Added call to zgetgd when no graphics device specified.
	 *             Changed call to begindevice to begindevices. (wct)
	 *    910220:  Move stmt label 8888, so lframe etc. are restored on err exit
	 *    880411:  Axes annotation now controlled by GEM variables.
	 *    850321:  Now displaying REL offset below FILEID.
	 *    821228:  Added calls to DISPID, DISPPK and PLHOME.
	 *    821122:  Added check for bad date fields.
	 *             Fixed bug involving titles and PP option.
	 *    820823:  Fixed bug involving extra x axes when using PP option.
	 *    820721:  Changed to newest set of parsing and checking functions.
	 *    811228:  Deleted call to ZCLIP.
	 *    810120:  Changed to output message retrieval from disk.
	 *    800920:  Added PERPLOT option.
	 *             Fixed bug in REL/ABS option.
	 *    800905:  Pick and file id options to new DISPLAY command.
	 *    800618:  Added pick display capability to this plot.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED: 
	 *===================================================================== */
	/* PROCEDURE: */
	/* Errors before plsave have to avoid going to execute plrest. */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){
	    /* -- "PERPLOT ON/OFF/n":  change number of files plotted per frame. */
	    if( lklogi( "PERPLOT$",9, &lperpl, &nperpl ) )
	    { /* do nothing */ }

	    /* -- "RELATIVE/ABSOLUTE":  change method of displaying time on x axis. */
	    else if( lclog2( "RELATIVE$",10, "ABSOLUTE$",10, &lrel ) )
	    { /* do nothing */ }

            /* if PRINT option is tried, get printer name */
            else if ( ltry ) {
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

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
	    goto L_8888;

	/* CHECKING PHASE: */

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Check to make sure all files are time series files. */

	vftime( nerr );
	if( *nerr != 0 ){
	    aplmsg( "Use PLOTSP command to plot spectral data.",42 );
	    goto L_8888;
	}

	/* - If no graphics device is open, try to open the default device. */

	getstatus( "ANY", &lany );
	if( !lany ){
	    zgetgd( kmgam.kgddef,9 );
	    begindevices( kmgam.kgddef,9, 1, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}

	/* EXECUTION PHASE: */

	/* - Save current plot and x limit attributes.
	 * - Error after plsave have to go to execute plrest. */

	plsave();

        /* initialize plot offsets */
        for ( i=0; i<MDFL; i++) toff[i] = 0.0;

	/* - Set up specific options that apply only to this plot. */

	lbotaxsave = cmgem.axis[BOTTOM].annotate;
	lbottcsave = cmgem.axis[BOTTOM].ticks;
	ltoptcsave = cmgem.axis[TOP].ticks;
	cmgem.axis[BOTTOM].ticks    = FALSE;
	cmgem.axis[BOTTOM].annotate = FALSE;

	lxlabsave = cmgem.xlabel.on;
	lylabsave = cmgem.ylabel.on;
	ltitlsave = cmgem.title.on;
	lxgrdsave = cmgem.lxgrd;
	cmgem.xlabel.on = FALSE;
	cmgem.ylabel.on = FALSE;
	cmgem.title.on  = FALSE;
	cmgem.lxgrd = FALSE;

	/* - Set up y window for each subplot. */

	if( lperpl ){
	    nfr = (cmdfm.ndfl - 1)/nperpl + 1;
	    nperfr = nperpl;
	}
	else{
	    nfr = 1;
	    nperfr = cmdfm.ndfl;
	}
	ypdel = (cmgem.plot.ymax - cmgem.plot.ymin)/(float)( nperfr );

	/* - Check WAIT option.  This is on when:
	 * -- A wait request has been made.
	 * -- An active device (normally the user's terminal) is on. */

	if( cmgam.lwaitr )
	    getstatus( "ACTIVE", &lwait );
	else
	    lwait = FALSE;

	/* - Loop on number of frames: */

	jdfl1 = 1;
	ypmxsave = cmgem.plot.ymax;
	lframesave = cmgem.lframe;
	for( jfr = 1; jfr <= nfr; jfr++ ){
	    /* set cmgem.lframe FALSE for each pass through loop, because 
		endframe() sets it back to TRUE for the next pass. */
	    cmgem.lframe = FALSE;

	    /* -- No wait after last frame. */
	    if( jfr == nfr && !cmgam.lwaite )
		lwait = FALSE;

	    /* -- Loop on data files in each frame: */

	    jdfl2 = min( cmdfm.ndfl, jdfl1 + nperfr - 1 );

	    /* -- Determine time limits for x axis of this frame.
	     *    (Correct for any differences in GMT reference time.) */

	    getfil( jdfl1, TRUE, &num, &nlcy, &nlcx, nerr );
	    if( *nerr != 0 )
		goto L_7777;
	    jperfr = 1;
	    getxlm( &lxlims, &tmin, &tmax );
/*            if( !lxlims ){	commented out to allow relative mode when xlim is set. maf 970723 */
		if( lrel ){
		    tmax = tmax - tmin;
		    Toff[jperfr] = -tmin;
		    tmin = 0.;
	    	}
	    	else{
		    copyi( nzdttm, n1dttm, 6 );
		    l1dttm = ldttm( n1dttm );
		    Toff[jperfr] = 0.;
	    	}
		for( jdfl = jdfl1 + 1; jdfl <= jdfl2; jdfl++ ){
		    jperfr = jperfr + 1;
		    getfil( jdfl, TRUE, &num, &nlcy, &nlcx, nerr );
		    if( *nerr != 0 )
			goto L_7777;
		    getxlm( &lxlims, &tminj, &tmaxj );
		    if( lrel ){
			tmax = fmax( tmax, tmaxj - tminj );
			Toff[jperfr] = -tminj;
		    }
		    else{
			if( l1dttm && ldttm( nzdttm ) ){
			    ddttm( nzdttm, n1dttm, &Toff[jperfr] );
			    /* if it starts 2 days after the first file,
				plot relative. maf 970908 */
			    if ( fabs ( Toff[jperfr] ) > TWODAYS )
				Toff[jperfr] = 0 ;
			}
			else{
			    Toff[jperfr] = 0.;
			}
			tmin = fmin( tmin, tminj + Toff[jperfr] );
			tmax = fmax( tmax, tmaxj + Toff[jperfr] );
		    } /* end else associated with if ( lrel ) */
		} /* end for( jdfl = jdfl1 + 1; jdfl <= jdfl2; jdfl++ ) */
/*	    }  end if ( !lxlims ) commented out to allow relative mode when xlim is set. maf 970723 */

	    /* - Check range of time limits to avoid errors that could occur
	     *   later during plotting. *

	    if( fabs( tmax - tmin ) > (float)( MLARGE ) ){
		*nerr = 1504;
		setmsg( "ERROR", *nerr );
		goto L_7777;
	    } */

	    /* - Set x axis plot limits. */

	    cmgem.lxlim = TRUE;
	    cmgem.ximn = tmin;
	    cmgem.ximx = tmax;

	    if( lframesave ){
		beginframe( lprint , nerr );
		if( *nerr != 0 )
		    goto L_7777;
		getvspace( &cmgem.view.xmin, &cmgem.view.xmax, 
                           &cmgem.view.ymin, &cmgem.view.ymax );
	    }
	    jperfr = 0;

	    cmgem.tsdef = fmin( cmgem.tsdef, (cmgem.view.ymax - cmgem.view.ymin)/(8.0*
	     (float)( nperfr )) );
	    cmgam.tsfid = cmgem.tsdef;
	    cmgam.tspk = cmgem.tsdef;
	    cmgem.tsaxis = cmgem.tsdef;

	    for( jdfl = jdfl1; jdfl <= jdfl2; jdfl++ ){
		jperfr = jperfr + 1;
		cmgem.plot.ymin = cmgem.plot.ymax - ypdel;

		/* --- Get pointers to this file's location in memory. */

		getfil( jdfl, TRUE, &num, &nlcy, &nlcx, nerr );
		if( *nerr != 0 )
		    goto L_7777;

		/* --- Set up x axis data values. */

		if( *leven ){
		    cmgem.xgen.on = TRUE;
		    cmgem.xgen.delta = *delta;
		    cmgem.xgen.first = *begin + Toff[jperfr];
		}
		else{
		    cmgem.xgen.on = FALSE;
		}

		/* --- Set up y axis plot limits. */

		getylm( &cmgem.lylim, &cmgem.yimn, &cmgem.yimx );

		/* --- Plot this file. */

		pl2d( cmmem.sacmem[nlcx], cmmem.sacmem[nlcy], num, 1, 1, nerr );
		if( *nerr != 0 )
		    goto L_7777;

		/* --- Plot picks and fileid. */

		disppk( Toff[jperfr] );
                
		/* --- Add a label with offset time if this is a REL plot. */
                kptext = NULL;
                n = 0;
		if( lrel && cmgam.lfidrq ){
                  asprintf(&kptext, "OFFSET: %10.3e", -Toff[jperfr] );
                  n = 1;
                }
		dispid( cmgam.lfinorq , jdfl, n, &kptext );
                if(kptext) {
                  free(kptext);
                  kptext = NULL;
                }
		cmgem.plot.ymax = cmgem.plot.ymin;
	    } 

	    /* -- Draw bottom x axis. */
	    cmgem.axis[BOTTOM].annotate = lbotaxsave;
	    cmgem.axis[BOTTOM].ticks    = lbottcsave;
	    cmgem.axis[TOP].ticks       = ltoptcsave;
	    cmgem.lxgrd = lxgrdsave;
	    cmgem.uplot.ymax = ypmxsave*cmgem.view.ymax;
	    cmgem.chht = cmgem.tsaxis;
	    cmgem.chwid = cmgem.txrat*cmgem.chht;
	    settextsize( cmgem.chwid, cmgem.chht );
	    if( cmgem.ixint == AXIS_LINEAR ){
		xlinax();
	    }
	    else if( cmgem.ixint == AXIS_LOG ){
		xlogax();
	    }

	    /* -- Draw axes labels and title. */
	    if( lxlabsave )
                centxt( kmgem.kxlab,145, cmgem.xlabel.len, cmgem.xlabel.pos, cmgem.xlabel.text_size );
	    if( lylabsave )
		centxt( kmgem.kylab,145, cmgem.ylabel.len, cmgem.ylabel.pos, cmgem.ylabel.text_size );
	    if( ltitlsave )
		centxt( kmgem.ktitl,145, cmgem.title.len, cmgem.title.pos, cmgem.title.text_size );

	    /* -- Home cursor, advance frame and restore some GEM parameters. */
	    plhome();
	    if( lframesave )
		endframe( FALSE , nerr );
            else
              flushbuffer( nerr );
	    cmgem.plot.ymax = ypmxsave;
	    cmgem.axis[BOTTOM].annotate = FALSE;
	    cmgem.axis[BOTTOM].ticks    = FALSE;

	    /* -- Wait for user prompt before plotting next frame if appropriate. */
	    if( lwait ){
		zgpmsg( kwait,9, kret,9 );
		ncret = indexb( kret,9 );
		upcase( kret, ncret, kret,9 );
		if( kret[0] == 'K' )
		    goto L_7777;
		if( kret[0] == 'G' )
		    lwait = FALSE;
	    }

	    jdfl1 = jdfl2 + 1;
	} /* end for ( jfr ) */

	/* - Restore plot and x limit attributes.  Return. */

L_7777:
	plrest();
	cmgam.tsfid = cmgem.tsdef;
	cmgam.tspk = cmgem.tsdef;
	cmgem.tsaxis = cmgem.tsdef;

	cmgem.plot.ymax = ypmxsave;
	cmgem.axis[BOTTOM].annotate = lbotaxsave;
	cmgem.axis[BOTTOM].ticks    = lbottcsave;
	cmgem.lframe = lframesave;

L_8888:
	return;
} /* end of function */

