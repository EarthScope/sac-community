
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "sss.h"
#include "co.h"
#include "tt.h"
#include "gdm.h"
#include "gem.h"
#include "gam.h"
#include "hdr.h"
#include "dfm.h"
#include "amf.h"
#include "bool.h"


#include "gtm.h"
#include "pl.h"
#include "bot.h"
#include "ucf.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "dff.h"

#define maxCropLevels 5

void xplotrecords(nerr)
int *nerr;
{
	char kpllab[MCPFN+7], 	/* Number of elements increased. Add room for jdfl. maf 970129 */
	     kvr[9];
	int lany, lbotaxsave, lbottcsave, lflip, lframesave, 
	 llefaxsave, lleftcsave, lmissd, lrigaxsave, lrigtcsave, ltopaxsave, 
	 ltoptcsave, lvlim, lvspacetype,
	 lprint = FALSE , ltry = FALSE ;
	int llims , n1dttm[6] ;  /* added to display picks. maf 961219 */
	float tmin, tmax , toff[MDFL] ;    /* " */  /* Toff are time offsets */
	float *const Toff = &toff[0] - 1 ; /* " */
	int kdx, kdx_, ioffsetdta, ioffsettw, jdx, 
	 jdfl, jvr, ndx1, ndx2, ndy1, nferr, nlen, notused, 
	 numplot, nvr;
	float angle, atime, delay, delsiz, dgscale, 
	 dstchn, dstmn, dstmx, dvr, fudge, rdist, rosetp, unused, unused_,
	 vbigr, vmn, vmx, vr, vspaceratio, xaxlen, xbp, xep, xfac, 
	 xpdel, xpllen, xpmid, xpmnsv, xpmxsv, xttint, xwloc, 
	 yaxlen, ybp, yep, ypllen, yttint, ywloc;
	float ypmnsv, ypmxsv, ypmid, yfac, ypdel, ypmidu; 

	/* Variables added to handle cropping and zooming, maf 960716 */
	int	cropLevel = 0 ,	/* # of times the user cropped plot for zoom */
		idx ;		/* index for loops */
/*	const	int maxCropLevels = 5 ;*/
	float	* storedLimits[maxCropLevels] ;	/* limits of all crop levels */
  char *tmp;
  double dtmp;

	/*=====================================================================
	 * PURPOSE:  To execute the action command PLOTRECORDSECTION.
	 *           This command makes a record section plot.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 5102, 5103
	 *=====================================================================
	 * MODULE/LEVEL:  sss/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    gem:      llefax, lrigax, ltopax, lbotax, lleftc, lrigtc, ltoptc,
	 *              lbottc, ltitl, lbdr, lxgrd, lygrd, ixint, ilin, iyint,
	 *              lylim, lxgen, lygen, lxlim, yimn, yimx, lframe, lxrev,
	 *              lyrev
	 *    gam:      kgddef, iul, iur, ill, ilr
	 *    dfm:      ndfl, kdfl
	 *    hdr:      fundef
	 *    sss:      dst, idwop, dwlim, idaop, dalen, dasca, axdel, lvm,
	 *              twlim, lpol, lroset, iroset, beginTime, endTime
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    sss:     lrslab, knmlab, lrswt, lrspol, lrslin, xpsize, laspect,
	 *             lOriginDefault (formerly llefor, maf 961004), Twlim,
	 *             lPlottingTT
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, lklog, cfmt, cresp, getstatus, begindevices,
	 *             setmsg, apcmsg, plsave, extrma, setsgfsize, inqvsp,
	 *             beginframe, rectangle, vmcalc, vmdly, 
	 *             pl2d, setlinestyle, line, settextsize, settextangle,
	 *             pltext, plmap, plgrid, cnvita, ljust, plhome, endframe
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    dstmn, dstmx, dstchn
	 *    xaxlen, yaxlen
	 *    xpllen, ypllen
	 *    xvspmn, xvspmx, yvspmn, yvspmx
	 *    delsiz
	 *    xpmnsv, xpmxsv, ypmnsv, ypmxsv
	 *    xfac
	 *    lvspacetype,vspaceratio
	 *=====================================================================
	 * KNOWN BUGS:
	 * - The DISTANCEWINDOW option for output in degrees rather than
	 *   kilometers has not been implemented.
	 * - The commands (TIMEAXIS and DISTANCEAXIS) which control the plot 
	 *   aspect ratio and size are implemented, but the SGF conversion
	 *   programs have not been modified to include logic (i.e. paneling)
	 *   to implement the plot size options.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    970809:  Removed ddttm() from pick determination.  maf
	 *    970129:  Plots file numbers (jdfl). maf
	 *    961004:  Modified to allow traveltime in orient reversed mode.
	 *    960829:  Modified to allow traveltime in portrait mode.
	 *    960701:  Allow the code to set a time window if none is defined.
	 *    920728:  Added reduced travel time plots. Are either relative to a 
	 *             wave or a velocity.
	 *    920727:  Added apparent velocity cursor.
	 *    920720:  Added travel time curves.
	 *    891130:  Fixed bugs involving scaling and clipping of data.
	 *    890924:  Allowed AXES, TICKS, and BORDER commands to control
	 *             relevant attributes for record section plot.
	 *             Also fixed bug with BEGINFRAME and ENDFRAME commands.
	 *             Also fixed bug with VSPACE control over aspect ratio.
	 *    890301:  Fixed bug when origin of plot was on the right.
	 *             XLABEL and YLABEL commands now control axes labels.
	 *    870112:  Fixed bug involving positioning of traces on plot.
	 *    860306:  Rewrote logic for determine both distance and time
	 *             axis limits and size.
	 *    850819:  Major rewrite of subprocess.
	 *    850801:  Changes in argument list for RDSAC.
	 *    821201:  Changed to latest set of parsing and checking functions.
	 *    811228:  Deleted call to ZCLIP.
	 *    810120:  Changed to output message retrieval from disk.
	 *    800513:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850819
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* initialize */
	ndx1 = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

	    /* -- "LABELS ON|OFF|headerfield":  plot with or without labels. */
	    if( lklogc( "LABELS$",8, &cmsss.lrslab, kmsss.knmlab,9 ) )
	    { /* do nothing */ }

	    /* -- "WEIGHTS ON|OFF":  turn weighting on or off. */
	    else if( lklog( "WEIGHTS$",9, &cmsss.lrswt ) )
	    { /* do nothing */ }

	    /* -- "CURSOR ON|OFF":  plot with or without cursor. */
	    else if( lklog( "CURSOR$",8, &cmsss.lrscur ) )
	    { /* do nothing */ }

	    /* -- "POLARITY ON|OFF":  plot with or without polarities. */
	    else if( lklog( "POLARITY$",10, &cmsss.lrspol ) )
	    { /* do nothing */ }

	    /* -- "REFERENCELINE ON|OFF":  plot with or without reference line. */
	    else if( lklog( "REFERENCELINE$",15, &cmsss.lrslin ) )
	    { /* do nothing */ }

	    /* -- "SIZE v": set size factor of each trace as fraction of plot size. */
	    else if( lkreal( "SIZE$",6, &dtmp ) ) {
        cmsss.xpsize = (float) dtmp;
      }

	    /* -- "ASPECT ON|OFF": set aspect preservation option. */
	    else if( lklog( "ASPECT$",8, &cmsss.laspect ) )
	    { /* do nothing */ }

	    /* -- "ORIGIN DEFAULT|REVERSED": set origin of plot to desired side.
		   default is top in portrait mode and right in landscape mode.
		   llefor became lOriginDefault.  modified by maf 961004 */
	    else if( lklog2( "ORIGIN$",8, "DEFAULT$",9, "REVERSED$",10, &cmsss.lOriginDefault ) )
	    { /* do nothing */ }

	    /* -- "ORIENT PORTRAIT|LANDSCAPE": Plot in portrait or landscape mode */
	    else if( lklog2( "ORIENT$",8, "PORTRAIT$",10, "LANDSCAPE$",11, &cmsss.lorient ) )
	    { /* do nothing */ }

	    /* -- "TTIME ON|OFF|DEFAULT|text": Turn travel time curves on/off */
	    else if( lckey( "TTIME$",7 ) ){
		if( lclog( &cmtt.lttm ) )
		{ /* do nothing */ }
		else if( lckey( "DEFAULT$",9 ) )
		    cmtt.lttm = TRUE;
	    }

	    /* -- "XLABEL ON|OFF|DEFAULT|text": Set x axis label properties. */
	    else if( lckey( "XLABEL$",8 ) ){
		if( lclog( &cmsss.lxlabreq ) )
		{ /* do nothing */ }
		else if( lckey( "DEFAULT$",9 ) ){
		    cmsss.lxlabreq = TRUE;
		    cmsss.lxlabdef = TRUE;
		}
		else if( lcquot( MCMSG, kmsss.kxlabreq,MCMSG+1, &notused ) ){
		    cmsss.lxlabreq = TRUE;
		    cmsss.lxlabdef = FALSE;
		}
	    }

	    /* -- "YLABEL ON|OFF|DEFAULT|text": Set y axis label properties. */
	    else if( lckey( "YLABEL$",8 ) ){
		if( lclog( &cmsss.lylabreq ) )
		{ /* do nothing */ }
		else if( lckey( "DEFAULT$",9 ) ){
		    cmsss.lylabreq = TRUE;
		    cmsss.lylabdef = TRUE;
		}
		else if( lcquot( MCMSG, kmsss.kylabreq,MCMSG+1, &notused ) ){
		    cmsss.lylabreq = TRUE;
		    cmsss.lylabdef = FALSE;
		}
	    }

	    /* -- "REDUCED ON | OFF | PHASE phase | VELOCITY velocity" */
	    else if( lckey( "REDUCED$",9 ) ){
		if( lclog( &cmtt.lrdtt ) )
		{ /* do nothing */ }
		else if( lclist( (char*)kmtt.kttrd,9, MTTRD, &cmtt.nttrd ) ){
		    /*  VELOCITY - Save the velocity */
		    cmtt.lrdtt = TRUE;
		    if( cmtt.nttrd == 1 ){
          lcreal( &dtmp );
          cmtt.rdvel = (float) dtmp;
		    }
		    else if( cmtt.nttrd == 2 ){
			/*  PHASE - Save the phase  */
			lcchar( MTTLEN, kmtt.krdph,9, &notused );
			/*  Search through the phase list to find the relative one */
			cmtt.nrdph = 0;
			for( kdx = 0; kdx < cmtt.nttm; kdx++ ){
			    if( memcmp(kmtt.krdph,kmtt.kttnm[kdx],notused) == 0 )
				cmtt.nrdph = kdx + 1 ;
			} /* end for */
			if( cmtt.nrdph == 0 )
			    cmtt.nttrd = 0;
		    } /* end else if( cmtt.nttrd == 2 ) */
		} /* end else if( lclist( ... ) ) */
	    } /* end else if( lckey( "REDUCED$",9 ) ) */

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
		cfmt( "BAD SYNTAX: see help plotrecordsection",40 );
		cresp();
	    }
	} /* end parsing loop:  while ( lcmore( nerr ) ) */

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	/* CHECKING PHASE: */

	/* - Set kilometers to degrees scale if we need to do the conversion */

	if( cmsss.ndwun == 1 ){
	    dgscale = 1.0;
	}
	else{
	    dgscale = 1.0/RKMPERDG;
	}

	/* - See which graphics devices are active. */

	getstatus( "ANY", &lany );
	if( !lany ){
	    begindevice( kmgam.kgddef,9, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}

	/* - Test for non-null stacklist.
	 *   (First file in stacklist is reserved for sum). */

	if( cmdfm.ndfl <= 0 && cmtt.lttm <= 0){
	    *nerr = 5102;
	    setmsg( "ERROR", *nerr );
	    goto L_8888;
	}


	/* - Check for traces with missing distances. */

	lmissd = FALSE;
	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
	    if( Dst[jdfl] == cmhdr.fundef ){
		if( !lmissd ){
		    *nerr = 5104;
		    setmsg( "WARNING", *nerr );
		    lmissd = TRUE;
		}
        tmp = string_list_get(datafiles, jdfl-1);
        apcmsg2(tmp, strlen(tmp)+1);
	    }
	} /* end for ( jdfl ) */
	if( *nerr != 0 )
	    goto L_8888;

	/* EXECUTION PHASE: */

	/* initiallize storedLimits, maf 960716 */
	for ( idx = 0 ; idx < maxCropLevels ; idx++ )
	    storedLimits[idx] = NULL ;

	/* - Save current plot parameters and restore them after plot. */

	plsave();

	/* - Store some plot parameters into local variables for use in creating plot. */

	llefaxsave = cmgem.axis[LEFT].annotate;
	lrigaxsave = cmgem.axis[RIGHT].annotate;
	ltopaxsave = cmgem.axis[TOP].annotate;
	lbotaxsave = cmgem.axis[BOTTOM].annotate;
	lleftcsave = cmgem.axis[LEFT].ticks;
	lrigtcsave = cmgem.axis[RIGHT].ticks;
	ltoptcsave = cmgem.axis[TOP].ticks;
	lbottcsave = cmgem.axis[BOTTOM].ticks;
	lframesave = cmgem.lframe;

	/* - Set up specific options for this plot only. */

	cmgem.axis[LEFT].annotate   = FALSE;
	cmgem.axis[RIGHT].annotate  = FALSE;
	cmgem.axis[TOP].annotate    = FALSE;
	cmgem.axis[BOTTOM].annotate = FALSE;
	cmgem.axis[LEFT].ticks      = FALSE;
	cmgem.axis[RIGHT].ticks     = FALSE;
	cmgem.axis[TOP].ticks       = FALSE;
	cmgem.axis[BOTTOM].ticks    = FALSE;
	cmgem.title.on = FALSE;
	cmgem.xlabel.on = FALSE;
	cmgem.ylabel.on = FALSE;
	cmgem.lbdr = FALSE;
	cmgem.lframe = FALSE;
	cmgem.lxgrd = FALSE;
	cmgem.lygrd = FALSE;
	cmgem.ixint = AXIS_LINEAR;
	cmgem.iyint = AXIS_LINEAR;
	cmgem.lylim = TRUE;
	cmgem.lxlim = TRUE;

	if ( cmsss.lorient ) {
	    cmgem.xgen.on = TRUE;
	    cmgem.ygen.on = FALSE;
	}
	else {
	    cmgem.xgen.on = FALSE;
	    cmgem.ygen.on = TRUE;
	}

	/* - Define distance window limits in km. */

	if( cmsss.idwop == 1 || cmsss.idwop == 2 ){
	    extrma( cmsss.dst, 1, cmdfm.ndfl, &dstmn, &dstmx, &unused );
	    dstmn = dstmn*dgscale;
	    dstmx = dstmx*dgscale;
	    dstchn = 0.15*(dstmx - dstmn);
	    dstmn = dstmn - dstchn;
	    if( cmsss.idwop == 1 )
		dstmx = dstmx + dstchn;
	    else
		dstmx = dstmn + cmsss.dwwid;
	}
	else if( cmsss.idwop == 3 ){
	    dstmn = Dwlim[1];
	    dstmx = Dwlim[2];
	}
	if( dstmx <= dstmn )
	    dstmx = dstmn + 1.0;


        /* - Define time axis limits in seconds. Overhauled 960701 maf */

        /* if the time window was not set by the user, find one and put it in Twlim. */
        if ( !cmsss.ltwlim ) {
            float	hedgeSize ;       /* the amount by which to pad the data in the plot */

            /* find the earliest begin time and the latest end time */
	    extrma( cmsss.beginTime, 1, cmdfm.ndfl, &Twlim[1], &unused, &unused_ );
	    extrma( cmsss.endTime, 1, cmdfm.ndfl, &unused, &Twlim[2], &unused_ );

            /* hedge the data by a small amount on either side */
            hedgeSize = 0.15*(Twlim[2] - Twlim[1]);
            Twlim[1] = Twlim[1] - hedgeSize;
            Twlim[2] = Twlim[2] + hedgeSize;

            /* don't let max < min. */
            if ( Twlim[2] <= Twlim[1] ) {
	        Twlim[2] = Twlim[1] + 1.0 ;
            } /* end if ( Twlim[2] <= Twlim[1] ) */

        } /* end if (!cmsss.ltwlim) */

        /* end code overhauled 960701 maf */


	/* allocate space in storedLimits and fill it with the appropriate values. maf 960716 */
	if ( ( storedLimits[0] = ( float * ) malloc ( 4 * sizeof ( float ) ) ) == NULL ){
	    /* error handling */
	    printf("error allocating storedLimits--xplotrecords\n");
	    *nerr = 301;
	    return ;
	}

	storedLimits[0][0] = Twlim[1] ;
	storedLimits[0][1] = Twlim[2] ;
	storedLimits[0][2] = dstmn ;
	storedLimits[0][3] = dstmx ;

	/********************************************************************************
	 * The following while loop is added to permit cropping and zooming. maf 960716 */

	while ( cropLevel >= 0 ) {	/* when cropLevel turns negative, end loop */


	    /* set time and distance window limits for this pass through the loop. maf 960716 */
	    Twlim[1] = storedLimits[cropLevel][0] ;
            Twlim[2] = storedLimits[cropLevel][1] ;
            dstmn = storedLimits[cropLevel][2] ;
            dstmx = storedLimits[cropLevel][3] ;

	    /* re-set these to FALSE so it will print without individual axes each time
	       through the loop. Note: they get set later in the while loop so that the
	       outer axes will print. maf 960724 finishing work started 960716 */
            cmgem.axis[LEFT].annotate   = FALSE;
            cmgem.axis[RIGHT].annotate  = FALSE;
            cmgem.axis[TOP].annotate    = FALSE;
            cmgem.axis[BOTTOM].annotate = FALSE;
            cmgem.axis[LEFT].ticks      = FALSE;
            cmgem.axis[RIGHT].ticks     = FALSE;
            cmgem.axis[TOP].ticks       = FALSE;
            cmgem.axis[BOTTOM].ticks    = FALSE;

            /* if lorient = TRUE, time is on the X axis, else time is on the Y axis */
            if ( cmsss.lorient ) {
		cmgem.ximn = Twlim[1];
		cmgem.ximx = Twlim[2];
            }
            else {
		cmgem.yimn = Twlim[1];
		cmgem.yimx = Twlim[2];
            }


	    /* - Distance axis size is either fixed or scaled to the window limits. */

	    if ( cmsss.lorient ) {
		if( cmsss.idaop == 1 )
		    yaxlen = cmsss.dalen;
		else if( cmsss.idaop == 2 )
		    yaxlen = (dstmx - dstmn)/cmsss.dasca;
	    }
	    else {
		if( cmsss.idaop == 1 )
		    xaxlen = cmsss.dalen;
		else if( cmsss.idaop == 2 )
		    xaxlen = (dstmx - dstmn)/cmsss.dasca;
	    }

	    /* - Time axis size is either fixed or scaled to the window limits. */

	    if ( cmsss.lorient ) {
		if( cmsss.itaop == 1 )
		    xaxlen = cmsss.talen;
		else
		    xaxlen = (Twlim[2] - Twlim[1])/cmsss.tasca;
	    }
	    else {
		if( cmsss.itaop == 1 )
		    yaxlen = cmsss.talen;
		else
		    yaxlen = (Twlim[2] - Twlim[1])/cmsss.tasca;
	    }

	    /* - Calculate the viewspace for this plot. */

	    xpllen = xaxlen + 2.0*cmsss.axdel;
	    ypllen = yaxlen + 2.0*cmsss.axdel;


	    if( cmsss.laspect ){
		getvspacetype( &lvspacetype, &vspaceratio );
		cmgdm.lvsful = FALSE ;
		cmgdm.vsrat = cmsss.lorient ? xpllen/ypllen : ypllen/xpllen ;
	    }

	    /* - Set the size of the output SGF. */

	    /*      call setsgfsize('FIXED',xpllen) */

	    /* - Begin new frame and get current viewspace. */

	    if( lframesave ){
		beginframe( lprint , nerr );
		if( *nerr != 0 )
		    goto L_7777;
	    }

	    getvspace( &cmgem.view.xmin, &cmgem.view.xmax, &cmgem.view.ymin, &cmgem.view.ymax );

	    /* - Convert axes lengths to the logical 0-1 plot window. */

	    if ( cmsss.lorient ) {
		delsiz = cmsss.axdel/ypllen;

		/* The following set the dotted lines and the header 
		   label in the right place but not the data. */
		/* ypmnsv = cmgem.view.ymin + delsiz; 
		ypmxsv = cmgem.view.ymax - delsiz;
		cmgem.xpmn = (cmgem.view.xmin + delsiz)/cmgem.view.xmax;
		cmgem.xpmx = (cmgem.view.xmax - delsiz)/cmgem.view.xmax; */

		ypmnsv = (cmgem.view.ymin + delsiz)/cmgem.view.ymax;
		ypmxsv = (cmgem.view.ymax - delsiz)/cmgem.view.ymax;
		cmgem.plot.xmin = cmgem.view.xmin + delsiz;
		cmgem.plot.xmax = cmgem.view.xmax - delsiz;
		yfac = (ypmxsv - ypmnsv)/(dstmx - dstmn);
	    }
	    else {
		delsiz = cmsss.axdel/xpllen;
		xpmnsv = cmgem.view.xmin + delsiz;
		xpmxsv = cmgem.view.xmax - delsiz;
		cmgem.plot.ymin = (cmgem.view.ymin + delsiz)/cmgem.view.ymax;
		cmgem.plot.ymax = (cmgem.view.ymax - delsiz)/cmgem.view.ymax;
		xfac = (xpmxsv - xpmnsv)/(dstmx - dstmn);
	    }

	  
	    /* - Define the x and y half-window for each subplot. */
	    /*    Probably only need one for each orientation */ 

	    if ( cmsss.lorient ) 
		ypdel = 0.05*(ypmxsv - ypmnsv);
	    else
		xpdel = 0.05*(xpmxsv - xpmnsv);

	    /* - Calculate velocity model delays. */

	    if( Lvm[1] ){
		vmcalc( 1, nerr );
		if( *nerr != 0 )
		    goto L_7777;
		vmdly( nerr );
		if( *nerr != 0 )
		    goto L_7777;
	    }

	    /* - Get time delays for the various signals if pick option is turned on. maf 961219 */
	    if ( cmgam.ldsppk ) {
		getfil ( 1 , TRUE, &nlen, &ndx1, &ndx2, nerr );
		if ( *nerr != 0 )
		    goto L_7777;
		if ( cmsss.lorient )
		    getxlm ( &llims , &tmin , &tmax );
		else
		    getylm ( &llims , &tmin , &tmax ) ;
		if( !llims ){
		    copyi( nzdttm, n1dttm, 6 );
		    Toff[1] = 0.;
		    if ( Lvm[1] )
			Toff[1] = Dlyvm[1] ;
		    for( jdfl = 2; jdfl <= cmdfm.ndfl; jdfl++ ){
			getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
			if( *nerr != 0 )
			    goto L_7777;
/*			if( l1dttm && ldttm( nzdttm ) )
took this out		    ddttm( nzdttm, n1dttm, &Toff[jdfl] );
maf 970809		else
*/			    Toff[jdfl] = 0.;

			if ( Lvm[1] )
			    Toff[jdfl] += Dlyvm[jdfl] ;
		    } /* end for */
		} /* end if (!llims) */
	    } /* end if ( cmgam.ldsppk ), maf 961219 */


	    /* - Plot each file in its own subplot region with framing and axes off. */

	    for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Only plot traces which are inside the current plot window. */
		rdist = Dst[jdfl]*dgscale;
		if( rdist < dstmn || rdist > dstmx )
		    continue ;

		/* -- Get file from memory manager. */
		getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
		if( *nerr != 0 )
		    goto L_7777;

		/* -- Adjust start time for reduced travel time plot */
		timeadj( rdist, &atime, nerr );

		/* -- Set up delay and compute intersection of file's data and plot's 
		 *    time windows.  This determines how many data points to plot. */
		delay = Dlyt[jdfl] + Dlyn[jdfl]*cmsss.del + ( Lvm[1] ? Dlyvm[jdfl] : 0 ) ;
		definelimits( Twlim[1], Twlim[2], *begin + delay - atime, *ennd + delay - atime, 
		 *delta, &ioffsettw, &ioffsetdta, &numplot );

		/* Adjust picks for reduced time. maf 961219 */
		if ( cmgam.ldsppk )
		    Toff[jdfl] -= atime ; 

		/* -- Set up scaling parameters. */
		*scale = 1.0;
		if( cmsss.lrswt )
		    *scale = Wt[jdfl]**scale;
		lflip = TRUE;
		if( cmsss.lrspol && !Lpol[jdfl] )
		    lflip = !lflip;
		if( cmsss.lOriginDefault )	/* llefor became lOriginDefault maf 961004 */
		    lflip = !lflip;

		/* -- Compute extrema in data. */
		getylm( &lvlim, &vmn, &vmx );
		fudge = 0.0001*(vmx - vmn);
		vmn = vmn - fudge;
		vmx = vmx + fudge;
		vbigr = fmax( fabs( vmn ), fabs( vmx ) );

		/* -- Compute location of mid-point for this trace. */
		/* use xpmid or ypmid depending on lorient */
		if( cmsss.lOriginDefault ){		/* llefor became lOriginDefault maf 961004 */
		    if ( cmsss.lorient ) 
			ypmid = ypmxsv - (rdist - dstmn)*yfac;
		    else 
			xpmid = xpmxsv - (rdist - dstmn)*xfac;
		}
		else{
		    if ( cmsss.lorient ) 
			ypmid = ypmnsv + (rdist - dstmn)*yfac;
		    else
			xpmid = xpmnsv + (rdist - dstmn)*xfac;
		}

		/* -- Compute x or y mapping for this trace. */
		if( lflip ){
		    if ( cmsss.lorient ) {
			cmgem.yimn = vmx;
			cmgem.yimx = vmn;
			cmgem.plot.ymin = ypmid - *scale*ypdel*cmsss.xpsize*vmx/vbigr;
			cmgem.plot.ymax = ypmid - *scale*ypdel*cmsss.xpsize*vmn/vbigr;
		    }
		    else {
			cmgem.ximn = vmx;
			cmgem.ximx = vmn;
			cmgem.plot.xmin = xpmid - *scale*xpdel*cmsss.xpsize*vmx/vbigr;
			cmgem.plot.xmax = xpmid - *scale*xpdel*cmsss.xpsize*vmn/vbigr;
		    }
		}
		else{
		    if ( cmsss.lorient ) {
			cmgem.yimn = vmx;
			cmgem.yimx = vmn;
			cmgem.plot.ymin = ypmid + *scale*ypdel*cmsss.xpsize*vmn/vbigr;
			cmgem.plot.ymax = ypmid + *scale*ypdel*cmsss.xpsize*vmx/vbigr;
		    }
		    else {
			cmgem.ximn = vmn;
			cmgem.ximx = vmx;
			cmgem.plot.xmin = xpmid + *scale*xpdel*cmsss.xpsize*vmn/vbigr;
			cmgem.plot.xmax = xpmid + *scale*xpdel*cmsss.xpsize*vmx/vbigr;
		    }
		}

		/* -- Adjust limits to clip trace to be inside amplitude viewport if necessary. */
		if ( cmsss.lorient ) {
		    if( cmgem.plot.ymin < ypmnsv ){
			cmgem.yimn = cmgem.yimx + (cmgem.yimn - cmgem.yimx)*(ypmnsv - 
			 cmgem.plot.ymax)/(cmgem.plot.ymin - cmgem.plot.ymax);
			cmgem.plot.ymin = ypmnsv;
		    }
		    if( cmgem.plot.ymax > ypmxsv ){
			cmgem.yimx = cmgem.yimn + (cmgem.yimx - cmgem.yimn)*(ypmxsv - 
			 cmgem.plot.ymin)/(cmgem.plot.ymax - cmgem.plot.ymin);
			cmgem.plot.ymax = ypmxsv;
		    }
		}
		else {

		    if( cmgem.plot.xmin < xpmnsv ){
			cmgem.ximn = cmgem.ximx + (cmgem.ximn - cmgem.ximx)*(xpmnsv - 
			 cmgem.plot.xmax)/(cmgem.plot.xmin - cmgem.plot.xmax);
			cmgem.plot.xmin = xpmnsv;
		    }
		    if( cmgem.plot.xmax > xpmxsv ){
			cmgem.ximx = cmgem.ximn + (cmgem.ximx - cmgem.ximn)*(xpmxsv - 
			 cmgem.plot.xmin)/(cmgem.plot.xmax - cmgem.plot.xmin);
			cmgem.plot.xmax = xpmxsv;
		    }
		}


		/* -- Plot this trace. */

		/* Compute delta and begin */ 

		if ( cmsss.lorient ) {
		    cmgem.xgen.delta = *delta;
		    cmgem.xgen.first = *begin + delay + *delta*(float)( ioffsetdta ) - atime;
		}
		else {
		    cmgem.ygen.delta = *delta;
		    cmgem.ygen.first = *begin + delay + *delta*(float)( ioffsetdta ) - atime;
		}

		if ( cmsss.lorient )
		    pl2d( (float*)&unused, cmmem.sacmem[ndx1]+ioffsetdta, numplot, 1, 1, nerr );
		else
		    pl2d( cmmem.sacmem[ndx1]+ioffsetdta, (float*)&unused, numplot, 1, 1, nerr );

		if( *nerr != 0 )
		    goto L_7777;

		/* display picks.  maf 961219 */
		if ( cmsss.lorient )		/* portrait mode */
		    disppk( Toff[jdfl] );
		else				/* landscape mode */
		    disppkLandscape( Toff[jdfl] );

		/* -- Label each subplot with header field if requested.
		 *    Label is put at end of trace. */

		/* Define y midpoint for labels in portrait mode */
		if ( cmsss.lorient ) {
		    ypmidu = (cmgem.uplot.ymax + cmgem.uplot.ymin)/2.0;
		}

		if( cmsss.lrslab ){
		    cmgem.chht = cmgem.tsdef*fmin( cmsss.xpsize, 1.0 );
		    cmgem.chwid = cmgem.txrat*cmgem.chht;
		    settextsize( cmgem.chwid, cmgem.chht );
		    settextjust( "l" , "b" );

		    if (cmsss.lorient ) {
			settextangle( TEXT_HORIZONTAL );
			ywloc = ypmidu;
			xwloc = cmgem.view.xmax*cmgem.plot.xmax + cmgem.chwid;
		    }
		    else {
			settextangle( TEXT_VERTICAL );
			xwloc = xpmid + 0.5*cmgem.chht;
			ywloc = cmgem.view.ymax*cmgem.plot.ymax + cmgem.chwid;
		    }

		    formhv( kmsss.knmlab,9, 3, kpllab,MCPFN+1, &nferr );
		    kpllab[ indexb( kpllab,MCPFN+1 ) ] = '\0' ;	/* these four lines */
		    if ( cmgam.lfinorq )
			sprintf( kpllab , "%s - %d", kpllab , jdfl ) ;/* modified. maf     */
		    pltext( kpllab, strlen(kpllab) + 1, xwloc, ywloc );	   /* 970129 */

		    if (cmsss.lorient ) {
			line( cmgem.view.xmax*cmgem.plot.xmin - cmgem.chwid, ypmidu, 
			      cmgem.view.xmax*cmgem.plot.xmin, ypmidu );
			line( cmgem.view.xmax*cmgem.plot.xmax + cmgem.chwid, ypmidu,
			      cmgem.view.xmax*cmgem.plot.xmax, ypmidu );
		    }
		    else {
			line( xpmid, cmgem.view.ymax*cmgem.plot.ymin - cmgem.chwid, 
			      xpmid, cmgem.view.ymax*cmgem.plot.ymin );
			line( xpmid, cmgem.view.ymax*cmgem.plot.ymax + cmgem.chwid, 
			      xpmid, cmgem.view.ymax*cmgem.plot.ymax );
		    }

		} /* end if( cmsss.lrslab ) */

		/* -- Draw a dotted line at x location of each subplot. */
		if( cmsss.lrslin ){
		    setlinestyle( LINE_STYLE_DOTTED );
			
		    if ( cmsss.lorient )
		        /* line( cmgem.uplot.xmin, ypmid, cmgem.uplot.xmax, ypmid );*/
		        line( cmgem.uplot.xmin, ypmidu, cmgem.uplot.xmax, ypmidu );
		    else 
			 line( xpmid, cmgem.uplot.ymin, xpmid, cmgem.uplot.ymax );

		    setlinestyle( LINE_STYLE_SOLID );
		} /* end if( cmsss.lrslin ) */

	    } /* end for( jdfl ) (very large loop within even larger while loop) */

	    /* - Reset text characteristics if trace label option was on. */

	    if( cmsss.lrslab ){
		cmgem.chht = cmgem.tsdef;
		cmgem.chwid = cmgem.txrat*cmgem.chht;
		settextsize( cmgem.chwid, cmgem.chht );
		settextangle( TEXT_HORIZONTAL );
		settextjust( "l" , "b" );
	    }

	    /* - Set text size for axes and labels. */

	    cmgem.chht = cmgem.tsaxis;
	    cmgem.chwid = cmgem.txrat*cmgem.chht;
	    settextsize( cmgem.chwid, cmgem.chht );

	    /* - Distance axis annotation. */

	    if (cmsss.lorient ) {
		cmgem.plot.ymin = ypmnsv;
		cmgem.plot.ymax = ypmxsv;
		cmgem.lylim = TRUE;
		cmgem.yimn = dstmn;
		cmgem.yimx = dstmx;
		cmgem.axis[LEFT].ticks    = lleftcsave;
		cmgem.axis[LEFT].annotate = llefaxsave;
		cmgem.axis[RIGHT].ticks   = lrigtcsave;
		if( cmsss.lrslab ){
		    cmgem.axis[RIGHT].annotate = FALSE;
		}
		else{
		    cmgem.axis[RIGHT].annotate = lrigaxsave;
		}
		cmgem.lyrev = cmsss.lOriginDefault ;	/* llefor became lOriginDefault maf 961004 */
		plmap( (float*)&unused, cmmem.sacmem[ndx1], 0, 1, 1, nerr );
		ylinax();
	    }
	    else {
		cmgem.plot.xmin = xpmnsv;
		cmgem.plot.xmax = xpmxsv;
		cmgem.lxlim = TRUE;
		cmgem.ximn = dstmn;
		cmgem.ximx = dstmx;
		cmgem.axis[BOTTOM].ticks    = lbottcsave;
		cmgem.axis[BOTTOM].annotate = lbotaxsave;
		cmgem.axis[TOP].ticks       = ltoptcsave;
		if( cmsss.lrslab ){
		    cmgem.axis[TOP].annotate = FALSE;
		}
		else{
		    cmgem.axis[TOP].annotate = ltopaxsave;
		}
		cmgem.lxrev = cmsss.lOriginDefault ;	/* llefor became lOriginDefault maf 961004 */
		plmap( cmmem.sacmem[ndx1], (float*)&unused, 0, 1, 1, nerr );
		xlinax();
	    }

	    /* - Time axis annotation. */

	    if ( cmsss.lorient ) {
		if( cmgem.lyrev ){
		    cmgem.lyrev = FALSE;
		    plmap( (float*)&unused, cmmem.sacmem[ndx1], 0, 1, 1, nerr );
		}
		cmgem.lxlim = TRUE;
		cmgem.axis[BOTTOM].ticks    = lbottcsave;
		cmgem.axis[BOTTOM].annotate = lbotaxsave;
		cmgem.axis[TOP].ticks       = ltoptcsave;
		cmgem.axis[TOP].annotate    = ltopaxsave;
		xlinax();
	    }
	    else {
		if( cmgem.lxrev ){
		    cmgem.lxrev = FALSE;
		    plmap( cmmem.sacmem[ndx1], (float*)&unused, 0, 1, 1, nerr );
		}
		cmgem.lylim = TRUE;
		cmgem.axis[LEFT].ticks     = lleftcsave;
		cmgem.axis[LEFT].annotate  = llefaxsave;
		cmgem.axis[RIGHT].ticks    = lrigtcsave;
		cmgem.axis[RIGHT].annotate = lrigaxsave;
		ylinax();
	    }

	    /* - Distance and Time axis labels. */

	    if( cmsss.lxlabreq ){
		if( cmsss.lxlabdef ){
		    if( cmsss.ndwun == 1 )
			fstrncpy( kmgem.kxlab, 144, "Distance (km)", 13 );
		    else
			fstrncpy( kmgem.kxlab, 144, "Distance (degrees)", 18 );
		}
		else
		    strscpy( kmgem.kxlab, kmsss.kxlabreq, 144 );

		cmgem.xlabel.len = indexb( kmgem.kxlab,145 );

		if (cmsss.lorient )
		    centxt( kmgem.kxlab,145, cmgem.xlabel.len, LEFT, cmgem.xlabel.text_size );
		else
		    centxt( kmgem.kxlab,145, cmgem.xlabel.len, BOTTOM, cmgem.xlabel.text_size );

	    } /* end if( cmsss.lxlabreq ) */

	    if( cmsss.lylabreq ){
		if( cmsss.lylabdef ){
		    subscpy( kmgem.kylab, 0, 15, 144, "Time (sec)   [VM");
		    if( Lvm[1] ){
			if( Ivm[1] == cmsss.irefr )
			    subscpy( kmgem.kylab, 16, -1, 144, "=REFR]" );
			else
			    subscpy( kmgem.kylab, 16, -1, 144, "=NMO]" );
		    }
		    else
			subscpy( kmgem.kylab, 16, -1, 144, " OFF]" );
		}
		else
		    strscpy( kmgem.kylab, kmsss.kylabreq, 144 );
		cmgem.ylabel.len = indexb( kmgem.kylab,145 );
		if (cmsss.lorient )
		    centxt( kmgem.kylab,145, cmgem.ylabel.len, BOTTOM, cmgem.ylabel.text_size );
		else 
		    centxt( kmgem.kylab,145, cmgem.ylabel.len, LEFT, cmgem.ylabel.text_size );
	    } /* end if( cmsss.lylabreq ) */


	    /*  Portrait mode still needs work beyond here!!! */

	    /* - Draw velocity rossette if requested. */

	    if( cmsss.lroset ){

		/* -- Define size of rossette lines in plot and input coordinates. */
		rosetp = delsiz;
		/* roseti = rosetp/cmgem.ympip1; */

		/* -- Compute midpoint of rossette. */
		if( cmsss.iroset == cmgam.iul ){
		    xbp = cmgem.plot.xmin + 0.2*rosetp;
		    ybp = cmgem.plot.ymax - 1.2*rosetp;
		}
		else if( cmsss.iroset == cmgam.ill ){
		    xbp = cmgem.plot.xmin + 0.2*rosetp;
		    ybp = cmgem.plot.ymin + 1.2*rosetp;
		}
		else if( cmsss.iroset == cmgam.iur ){
		    xbp = cmgem.plot.xmax - 1.2*rosetp;
		    ybp = cmgem.plot.ymax - 1.2*rosetp;
		}
		else{
		    xbp = cmgem.plot.xmax - 1.2*rosetp;
		    ybp = cmgem.plot.ymin + 1.2*rosetp;
		}

		/* -- Draw vertical bar. */
		line( xbp, ybp - 0.5*rosetp, xbp, ybp + 0.5*rosetp );

		/* -- Compute velocities for rossette. */
		if( Lvm[1] ){
		    vr = (float)( (int)( Vapp[1] ) ) - 2.;
		    dvr = 1.;
		    nvr = 5;
		}
		else{
		    vr = 1.;
		    dvr = 2.;
		    nvr = 5;
		}

		/* -- Loop to draw velocity lines. */
		for( jvr = 1; jvr <= nvr; jvr++ ){
		    if ( cmsss.lorient ) {	/* Added to allow portrait mode, maf 960829 */
			if( Lvm[1] )
			    angle = atan( (Vapp[1] - vr)*(cmgem.ympip1/cmgem.xmpip1) );
			else
			    angle = atan( cmgem.ympip1/(cmgem.xmpip1/vr) );
		    } /* end if ( lorient ) */
		    else {
			if( Lvm[1] )
			    angle = atan( (1./Vapp[1] - 1./vr)*(cmgem.ympip1/cmgem.xmpip1) );
			else
			    angle = atan( cmgem.ympip1/(cmgem.xmpip1*vr) );
		    } /* end else associated with if ( lorient ) */

		    /* -- Compute endpoint for line. */
		    xep = xbp + rosetp*cos( angle );
		    yep = ybp + rosetp*sin( angle );

		    /* -- Draw and label line. */
		    line( xbp, ybp, xep, yep );
		    cnvita( (int)( vr ), kvr,9 );
		    ljust( kvr,9 );
		    cmgem.chht = cmgem.tsdef;
		    cmgem.chwid = cmgem.txrat*cmgem.chht;
		    settextsize( cmgem.chwid, cmgem.chht );
		    pltext( kvr,9, xep + cmgem.chwid, yep );
		    vr = vr + dvr;
		} /* end for ( jvr ) */
	    } /* end if( cmsss.lroset ) */

	    /* - Calculate and plot second velocity model line. */
		/* These 10 lines have not been updated for portrait mode. maf */
	    if( Lvm[2] ){
		vmcalc( 2, nerr );
		if( *nerr != 0 )
		    goto L_7777;
		/* I added lxgen on the following line.  It will be necessary 
		   to update this code for portrait mode.  maf 970808 */
		cmgem.xgen.on = cmgem.ygen.on = FALSE;
		vmline( dstmn, dstmx, nerr );
		if( *nerr != 0 )
		    goto L_7777;
	    }

	    /* - Plot traveltime curves if requested */

	    if( cmtt.lttm ){
		cmgem.lnull = TRUE;
		cmgem.vnull = -1.0;

		/* I consolidated the way lxrev and lyrev are handled.  These set the
		   origin to a default or reversed position.  maf 970808.  */
		if ( cmsss.lOriginDefault ) {
		    if ( cmsss.lorient )
			cmgem.lyrev = TRUE ;
		    else
			cmgem.lxrev = TRUE ;
		} /* end if ( cmsss.lOriginDefault ) */

		for( kdx = 1; kdx <= cmtt.nttm; kdx++ ){
		    kdx_ = kdx - 1;
		    /* I consolidated the way lxgen and lygen are handled. maf 970808 */
		    if ( cmtt.lpreviousModel )
			cmgem.xgen.on = cmgem.ygen.on = FALSE ;
		    else if ( cmsss.lorient ) {
                        cmgem.xgen.on = FALSE;
                        if( Ltteven[kdx] ){
                            cmgem.ygen.on = TRUE;
                            cmgem.ygen.first = Xttfirst[kdx]*dgscale;
                            cmgem.ygen.delta = Xttdel[kdx]*dgscale;
                        }
                        else{
                            cmgem.ygen.on = FALSE;
                        }
		    }
		    else {
			cmgem.ygen.on = FALSE;
			if( Ltteven[kdx] ){
			    cmgem.xgen.on = TRUE;
			    cmgem.xgen.first = Xttfirst[kdx]*dgscale;
			    cmgem.xgen.delta = Xttdel[kdx]*dgscale;
			}
			else{
			    cmgem.xgen.on = FALSE;
			}
		    }

		    if( cmtt.lrdtt && (cmtt.nttrd > 0) ){
                        allamb( &cmmem, Nttpt[kdx], &ndy1, nerr );
                        if( *nerr != 0 )
                            goto L_7777;
                        for( jdx = 1; jdx <= Nttpt[kdx]; jdx++ ){
                            if( *(cmmem.sacmem[Ndxtty[kdx]]+jdx-1) != cmgem.vnull ){
				if( Ltteven[kdx] )
                                    rdist = Xttfirst[kdx] + (jdx - 1)*Xttdel[kdx];
				else
                                    rdist = *(cmmem.sacmem[Ndxttx[kdx]] + jdx - 1);
				timeadj( rdist, &atime, nerr );
				*(cmmem.sacmem[ndy1]+jdx-1) = *(cmmem.sacmem[Ndxtty[kdx]]+jdx-1) - atime;
			    }
			    else
				*(cmmem.sacmem[ndy1]+jdx-1) = cmgem.vnull;
			}

	/* The following section was overhauled to allow portrait mode in prs, maf 960829  *
	   It was re-overhauled (model worked, files didn't) maf 970808 */

                        cmsss.lPlottingTT = TRUE ;

			if ( cmsss.lorient /*== cmtt.lpreviousModel*/ ) {
			    pl2d( cmmem.sacmem[ndy1], cmmem.sacmem[Ndxttx[kdx]], Nttpt[kdx],
                             1, 1, nerr );
			} /* end if ( cmsss.lorient == cmtt.lpreviousModel ) */
			else {
			    pl2d( cmmem.sacmem[Ndxttx[kdx]], cmmem.sacmem[ndy1], Nttpt[kdx], 
			     1, 1, nerr );
			} /* end else associated with if ( cmsss.lorient ) */
			cmsss.lPlottingTT = FALSE ;
			ttint( cmmem.sacmem[Ndxttx[kdx]], cmmem.sacmem[ndy1], Nttpt[kdx],
			 &xttint, &yttint, nerr );
			relamb( cmmem.sacmem, ndy1, nerr );
		    } /* end if ( cmtt.lrdtt && (cmtt.nttrd > 0) ) */


		    else{
			cmsss.lPlottingTT = TRUE ;

			if ( cmsss.lorient /*== cmtt.lpreviousModel*/ ) {
			    pl2d( cmmem.sacmem[Ndxtty[kdx]], cmmem.sacmem[Ndxttx[kdx]], Nttpt[kdx], 
			     1, 1, nerr );
			} /* end if ( cmsss.lorient == cmtt.lpreviousModel ) */
			else {
                            pl2d( cmmem.sacmem[Ndxttx[kdx]], cmmem.sacmem[Ndxtty[kdx]], Nttpt[kdx],
                             1, 1, nerr );
                        } /* end else associated with if ( cmss.lorient ) */
			cmsss.lPlottingTT = FALSE ;
			ttint( cmmem.sacmem[Ndxttx[kdx]], cmmem.sacmem[Ndxtty[kdx]], Nttpt[kdx],
			 &xttint, &yttint, nerr );
		    } /* end else associated with if ( cmtt.lrdtt && (cmtt.nttrd > 0) ) */
	/* end code overhauled 960829 , and 970808 */

		    if( *nerr != 0 )
				goto L_7777;

		    /* -- Label traveltime curve. */
                    cmgem.chht = cmgem.tsdef;
                    cmgem.chwid = cmgem.txrat*cmgem.chht;
                    settextsize( cmgem.chwid, cmgem.chht );

		    /* modified to allow portrait mode. maf 960829 */
		    /* modified to allow orient reversed. maf 961004 */
		    /* modified to label only curves from the models.  maf 970808 */
		    if ( kmtt.kphaseNames ) {
                        if ( cmsss.lorient /*&& cmtt.lpreviousModel*/ )
                        {
                            if ( cmsss.lOriginDefault )
                                pltext( kmtt.kphaseNames[kdx_],
					strlen ( kmtt.kphaseNames[kdx_] ) + 1,
					yttint , xttint - cmgem.chwid );
                            else
                                pltext( kmtt.kphaseNames[kdx_],
					strlen ( kmtt.kphaseNames[kdx_] ) + 1,
					yttint , xttint + cmgem.chwid );
                        }
                        else
                        {
                            if ( cmsss.lOriginDefault )
                                pltext( kmtt.kphaseNames[kdx_],
					strlen ( kmtt.kphaseNames[kdx_] ) + 1,
					xttint - cmgem.chwid , yttint );
                            else
                                pltext( kmtt.kphaseNames[kdx_],
					strlen ( kmtt.kphaseNames[kdx_] ) + 1,
					xttint + cmgem.chwid/2 , yttint );
                        }
		    }
		    else {
		        if ( cmsss.lorient /*&& cmtt.lpreviousModel*/ )
                        {
			    if ( cmsss.lOriginDefault )
                                pltext( (char*)kmtt.kttnm[kdx_],6,
					yttint , xttint - cmgem.chwid );
			    else
			        pltext( (char*)kmtt.kttnm[kdx_],6,
					yttint , xttint + cmgem.chwid );
                        }
		        else
		        {
			    if ( cmsss.lOriginDefault )
			        pltext( (char*)kmtt.kttnm[kdx_],6,
					xttint - cmgem.chwid , yttint );
			    else
			        pltext( (char*)kmtt.kttnm[kdx_],6,
					xttint + cmgem.chwid/2 , yttint );
		        }
		    }
flushbuffer( nerr ) ;
		} /* end for( kdx = 1; kdx <= cmtt.nttm; kdx++ ) */
	    } /* if( cmtt.lttm ) */

	    /* this section overhauled, maf 960716 */
	    if( cmsss.lrscur ) {
		int	nextLevel,  /* indicates the next available crop level */
			nextMove ;  /* 1 means zoom in, -1 means unzoom, else quit xplotrecords */

		nextLevel = ( cropLevel + 1 > maxCropLevels ? maxCropLevels : cropLevel + 1 ) ;
		if ( storedLimits[nextLevel] != NULL ){
		    free ( storedLimits[nextLevel] ) ;
		    storedLimits[nextLevel] = NULL ;
		}
		rscursor( &storedLimits[nextLevel] , &nextMove, nerr );
		if ( *nerr )
		    goto L_7777 ;
		if ( nextMove == 1 )
		    cropLevel = nextLevel ;
		else if ( nextMove == 0 )
		    cropLevel = -1 ;
		else if ( nextMove == -1 ) {
		    if ( cropLevel > 0 ){
		    	free ( storedLimits[cropLevel] ) ;
		    	storedLimits[cropLevel] = NULL ;
		    	cropLevel-- ;
		    }
		}
	    } /* end if ( cmsss.lrscur ) */
	    else
		cropLevel = -100 ;


	} /* end while */

	/* End the while loop between possibly cropped and zoomed plots. maf 960716 *
	 ****************************************************************************/

L_7777:
	/* Return time window to its original values , maf 960716 */
        Twlim[1] = storedLimits[0][0] ;
        Twlim[2] = storedLimits[0][1] ;

	/* Be sure to free up all the memory allocated to storedLimits. maf 960716 */
	for ( idx = maxCropLevels - 1 ; idx >= 0 ; idx-- ) {
	    if ( storedLimits[idx] != NULL )
		free ( storedLimits[idx] ) ;
	} /* end for */

        /* - Home cursor and perform new frame action.. */
        if( lframesave ){
            plhome();
            endframe( FALSE , nerr );
        } else {
          flushbuffer( nerr );
        }

	/* - Restore plot parameters and plot device status. */
	plrest();
	if( cmsss.laspect )
	    setvspacetype( lvspacetype, vspaceratio );
	/*      call setsgfsize('NORMAL',notused) */

L_8888:
	return;

} /* end of function */

