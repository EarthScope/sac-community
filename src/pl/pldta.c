
#include <stdlib.h>

#include "co.h"
#include "mach.h"
#include "pl.h"
#include "gem.h"
#include "gdm.h"
#include "bool.h"
#include "gtm.h"
#include "bot.h"
#include "ucf.h"

#define	MBLOCK	100

float data_to_view_x(float x) { return cmgem.xmpip2 + cmgem.xmpip1 * x; }
float data_to_view_y(float y) { return cmgem.ympip2 + cmgem.ympip1 * y; }

void 
pldta (float xarray[], float yarray[], int number, int incx, int incy, int *nerr)
{
	char kinc[9];
	int nXdata = 0 , nYdata = 0 ; /* number of points plotted so far, maf 980116 */
	int lclip, lfdon, lnewdp, lqdp, ltdon;
	int inc, insym, j, j3, 
	 jblock, jcopy, jx, jx1, jx2, jxb, 
	 jxf, jxl, jxx, jy, jy1, jy2, jyb, jyf, jyl, jyy, nblinbuf, 
	 nblock, ncopy, ncopyd, ninc, nqdp, nremdr, numf, numl, numu, 
	 nwhole, do_count;
	float slen, x1, x2, xblock[MBLOCK + 2], xrectangle, 
	 xtest, y1, y2, yblock[MBLOCK + 2], yrectangle, ytest;

	static float skfudge = 0.00053;

	float *const Xarray = &xarray[0] - 1;
	float *const Xblock = &xblock[0] - 1;
	float *const Yarray = &yarray[0] - 1;
	float *const Yblock = &yblock[0] - 1;


	/*=====================================================================
	 * PURPOSE: To plot a set of x-y data in the current frame.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    XARRAY:  Array of x data. [f]
	 *    YARRAY:  Array of y data. [f]
	 *    NUMBER:  Number of points to plot. [i]
	 *    INCX:    Increment in XARRAY array between points to plot.[i]
	 *    INCY:    Increment in YARRAY array between points to plot. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred. [i]
	 *             Potential error numbers: 0902.
	 *=====================================================================
	 * MODULE/LEVEL:  GEM/3
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GEM:     LTQDP, NTQDP, NFQDP, NTQDP,
	 *             LYLIM, LXLIM, LXGEN, LYGEN, XFIRST, XDELTA, XIMNU, YIMNU,
	 *             YFIRST, YDELTA, ICLINE, ICOL, IXINT, ILOG,
	 *             ISKLIN, LLINE, LILINE, IILINE, NILINE, JILINE,
	 *             ISKCOL, IICOL, NICOL, JICOL,
	 *             LSYM, LISYM, IISYM, NISYM, JISYM,
	 *             TSDEF, CHWID, CHHT,
	 *             LWIDTH, IWIDTH, ITHIN, ISKWIDTH, LIWIDTH, IIWIDTH
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GEM:     ILINE, ICOL, ISYM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  INIGSF, SETLINESTYLE, SETCOLOR, LOGDTA, PLCLIP, PLNOCL,
	 *             INCAT, SETSYMBOLNUM, SETTEXTSIZE, CNVITA, LJUST, LINE, PLTEXT,
	 *             FLUSHBUFFER, SETLINEWIDTH
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    LCLIP:   .TRUE. if clipping is on. [l]
	 *    LQDP:    .TRUE. if "quick and dirty plotting" (QDP) is on. [l]
	 *    NQDP:    Approximate number of points to plot if QDP is on. [i]
	 *    XBLOCK:  Storage for current block of X data being processed. [f]
	 *    YBLOCK:  Storage for current block of Y data being processed. [f]
	 *    MBLOCK:  Length of blocks (chomp size). [i]
	 *             MUST be an EVEN number for QDP logic to work properly.
	 *    NUMF:    Index of first data point to be plotted. [i]
	 *    NUML:    Index of last data point to be plotted. [i]
	 *    NUMU:    Actual number of data points to be plotted. [i]
	 *    INC:     Number of data points in each QDP section. [i]
	 *             The minimum and maximum values in each section are
	 *             computed and plotted when QDP option is on.
	 *    NWHOLE:  Number of whole blocks of data to process. [i]
	 *    NREMDR:  Number of points in partial block. [i]
	 *    NCOPY:   Total number of blocks to process. [i]
	 *    NBLOCK:  Number of points in current block.
	 *    NCOPYD:  Number of points that have been processed. [i]
	 *    JYB:     Index to first Y point in current section (when QDP
	 *             is on) or current block (when QDP off.) [i]
	 *    YBL1:    First Y value of block when Y data is "generated." [f]
	 *    JY1:     Index to first Y data point in section to plot. [i]
	 *    JY2:     Index to second Y data point in section to plot. [i]
	 *    JYF:     Index to first Y data point in QDP do loop. [i]
	 *    JYL:     Index to last Y data point in QDP do loop. [i]
	 *    JXB:     See definition for JYB.
	 *             Also applies to XBL1, JX1, JX2, JXF, JXL.
	 *    XSKFUDGE:Offset appropriate for thick lines on plot border.
	 *    YSKFUDGE:Offset appropriate for thick lines on plot border.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    980116:  Fixed bug with points running off the right side
	 *             of the plot.  maf
	 *    970225:  Removed a -1 from the evaluation of numl in the cases of
	 *             cmgem.lxgem or cmgem.lygem, effectively increasing the
	 *             value of numl by 1.  This to make plots display the 
	 *             upper limit as defined by the xlim command.  maf
	 *    960829:  Added check for NULL x values to support portrait mode.
	 *    920526:  Added line-width setting. TEXT is always thin!
	 *    910301:  Changed iline to icline.
	 *    910229:  Took out a line delcaring common /cmgdm/ lgdon(mgd),
	 *             igdtyp(mgd). It's handled in /inc/gem. Don't know
	 *             when the common declaration was added here, but it
	 *             gave compiler erors on Stellar. Added include gdm.
	 *    910207:  Set lvsclip before calling plclip/plnocl
	 *             add batching for SunView, and param/common for lgdon
	 *    900703:  Floor for log plots uses FLOOR if positive.
	 *             Otherwise it uses the minimum axis value.
	 *    860404:  Floor for log plots is now minimum axis value.
	 *             This value (XIMNU or YIMNU) is calculated in PLMAP.
	 *    850814:  Clipping now on if we have logarithmic interpolation.
	 *    850220:  Improved QDP logic so that minimum and maximum
	 *             are computed in each section.  Also added logic
	 *             to allow separate QDP option for SGF device.
	 *    830926:  Moved QDP logic from PL2D to here.
	 *    830810:  Replaced call to ZIMOVE with do-loop.
	 *    820720:  Modified clipping on/off logic.
	 *    820330:  Added linestyle logic.
	 *    820305:  Added color logic.
	 *    811118:  Fixed bug involving fixed limits and QDP option.
	 *    811030:  Modified logic for clipping of evenly spaced data.
	 *    810120:  Changed to output message retrieval from disk.
	 *    810107:  Fixed bug when MOD(NUMBER,MBLOCK)=0.
	 *    800724:  Added logic for skipping blocks of data if outside window.
	 *             Improved logic for determining good QDP increments.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850220
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Check "quick and dirty plot" option.  This option is on if:
	 *   (1) Terminal QDP option on and TERM device is active.  OR
	 *   (2) SGF QDP option is on and SGF device is active.  AND
	 *   (3) Either X or Y data is not "generated." */

	if( cmgem.ltqdp )
	    getstatus( "ACTIVE", &ltdon );
	if( cmgem.lfqdp )
	    getstatus( "PASSIVE", &lfdon );
	if( cmgem.ltqdp && ltdon ){
	    lqdp = TRUE;
	    nqdp = cmgem.ntqdp;
	}
	else if( cmgem.lfqdp && lfdon ){
	    lqdp = TRUE;
	    nqdp = cmgem.nfqdp;
	}
	else
	    lqdp = FALSE;

	if( cmgem.xgen.on && cmgem.ygen.on )
	    lqdp = FALSE;

	/* - Turn clipping flag on if:
	 *   (1) user requested clipping OR
	 *   (2) we have fixed y limits OR
	 *   (3) we have fixed x limits and data is not evenly spaced OR
	 *   (4) we have logarithmic interpolation in either direction. */

	lclip = (((cmgem.lrqclp || cmgem.lylim) || 
                  (cmgem.lxlim && !cmgem.xgen.on)) || 
                 cmgem.ixint == AXIS_LOG || 
                 cmgem.iyint == AXIS_LOG );
        
	/* - Update plot to all devices. */

	flushbuffer( nerr );

	/* - Set local values for the first, last and total number of data
	 *   points to plot. */

	numf = 1;
	numl = number;

	/* - If the x or y data is being generated, some simple tests may
	 *   eliminate data outside the plot window. */

	if( cmgem.xgen.on ){
	    xtest = cmgem.xgen.first + VSMALL;
	    if( xtest < cmgem.data.xmin )
		numf = 1 + (int)( (cmgem.data.xmin - xtest)/cmgem.xgen.delta );
	    xtest = cmgem.xgen.first + (float)( number - 1 )*cmgem.xgen.delta - VSMALL;
	    if( xtest > cmgem.data.xmax )
		numl = number - (int)( (xtest - cmgem.data.xmax)/cmgem.xgen.delta );
	}
	else if( cmgem.ygen.on ){
	    ytest = cmgem.ygen.first + VSMALL;
	    if( ytest < cmgem.data.ymin )
		numf = 1 + (int)( (cmgem.data.ymin - ytest)/cmgem.ygen.delta );
	    ytest = cmgem.ygen.first + (float)( number - 1 )*cmgem.ygen.delta - VSMALL;
	    if( ytest > cmgem.data.ymax )
		numl = number - (int)( (ytest - cmgem.data.ymax)/cmgem.ygen.delta );
	}

	numu = numl - numf + 1;

	/* fix a bug wherein the points plotted off the right side of the plot.
	   maf 980116 */
	nXdata = nYdata = numf - 1 ;

	/* - Calculate array increment if "quick and dirty plot" option is on.
	 *   (Factors of 2 below are because two data points, a minimum
	 *   and a maximum value, are computed for each section.) */

	inc = 1;
	if( lqdp )
	    inc = max( 1, 2*numu/nqdp );
	lqdp = inc > 2;
	if( lqdp )
	    numu = 2*numu/inc;

	/* - Copy blocks of data into local storage. */

	/*   The "remainder" is copied first, then the whole blocks of data.
	 *   The maximum block length is a local parameter (MBLOCK).
	 *   The blocks are overlapped by 1 point to connect all interior points. */

	/* -- Determine how many blocks and the size of the "remainder".
	 *    Force the remainder to be an even number for QDP logic. */

	nwhole = (numu - 2)/MBLOCK;
	nremdr = numu - MBLOCK*nwhole;
	if( nremdr > 1 ){
	    ncopy = nwhole + 1;
	    if( lqdp ){
		nblock = 2*(nremdr/2);
	    }
	    else{
		nblock = nremdr;
	    }
	}
	else{
	    ncopy = nwhole;
	    nblock = MBLOCK + 1;
	}

	/* - Set up indices for first block. */

	ncopyd = 0;
	jxb = numf;
	jyb = numf;

	/* - Set line display attributes: */

	/* -- linestyle */
	setlinestyle( cmgem.icline );

	/* -- line color */
	if( cmgem.lcol ) {
          setcolor( cmgem.icol );
        } else {
          setcolor( color_foreground_default() );
        }

	/* -- line-width */
	setlinewidth( cmgem.iwidth );

	/* -- Start of loop on blocks of data:
	 *    (LNEWDP is flag used in plotting scaled symbols) */


	lnewdp = TRUE;
	nblinbuf = 0;

	for( jcopy = 1; jcopy <= ncopy; jcopy++ ){
	    /* --- Either move the data block to local storage or generate it. */

	    if( lqdp ){
		if( !cmgem.ygen.on ){
		    for( jblock = 1; jblock <= nblock; jblock += 2 ){
			jy1 = jyb;
			jy2 = jyb;
			jyf = jyb + incy;
			jyl = jyb + incy*(inc - 1);
			do_count = (max(jyf,jyl)-min(jyf,jyl)+1)/labs(incy);
			for( jy = jyf; do_count > 0; jy += incy, do_count-- ){
			    if( Yarray[jy] < Yarray[jy1] )
				jy1 = jy;
			    if( Yarray[jy] > Yarray[jy2] )
				jy2 = jy;
			}
			if( jy2 < jy1 ){
			    j = jy1;
			    jy1 = jy2;
			    jy2 = j;
			}
			Yblock[jblock] = Yarray[jy1];
			Yblock[jblock + 1] = Yarray[jy2];
			if( cmgem.xgen.on ){
			    Xblock[jblock] = cmgem.xgen.first + (float)( jy1 - 1 )*cmgem.xgen.delta;
			    Xblock[jblock + 1] = cmgem.xgen.first + (float)( jy2 - 1 )*cmgem.xgen.delta;
			}
			else{
			    Xblock[jblock] = Xarray[jy1];
			    Xblock[jblock + 1] = Xarray[jy2];
			}
			jyb = jyb + incy*inc;
		    } /* end for ( jblock ) */
		} /* end if ( !cmgem.lygen ) */
		else if( !cmgem.xgen.on ){
		    for( jblock = 1; jblock <= nblock; jblock += 2 ){
			jx1 = jxb;
			jx2 = jxb;
			jxf = jxb + incx;
			jxl = jxf + incx*(inc - 1);
			do_count = (max(jxf,jxl)-min(jxf,jxl)+1)/labs(incx);
			for( jx = jxf; do_count > 0; jx += incx, do_count-- ){
			    if( Xarray[jx] < Xarray[jx1] )
				jx1 = jx;
			    if( Xarray[jx] > Xarray[jx2] )
				jx2 = jx;
			}
			if( jx2 < jx1 ){
			    j = jx1;
			    jx1 = jx2;
			    jx2 = j;
			}
			Xblock[jblock] = Xarray[jx1];
			Xblock[jblock + 1] = Xarray[jx2];
			Yblock[jblock] = cmgem.ygen.first + (float)( jx1 - 1 )*cmgem.ygen.delta;
			Yblock[jblock + 1] = cmgem.ygen.first + (float)( jx2 - 1 )*cmgem.ygen.delta;
			jxb = jxb + incx*inc;
		    } /* end for ( jblock ) */
		} /* end else if( !cmgem.lxgen ) */
	    } /* end if( lqdp ) */
	    else{
		if( cmgem.xgen.on ){
		    /* Modified to get Xblock using multiplication instead of addition
			to avoid rounding errors.  maf 980116 */
		    for( jblock = 1; jblock <= nblock; jblock++ , nXdata++ )
			Xblock[jblock] = cmgem.xgen.first + (float) nXdata * cmgem.xgen.delta;
		    nXdata -= 2 ; /* there's a two point overlap between blocks. */
		}
		else{
		    jxx = jxb;
		    for( jblock = 1; jblock <= nblock; jblock++ ){
			Xblock[jblock] = Xarray[jxx];
			jxx = jxx + incx;
		    }
		}
		if( cmgem.ygen.on ){
		    /* Modified to get Xblock using multiplication instead of addition
			to avoid rounding errors.  maf 980116 */
		    for( jblock = 1; jblock <= nblock; jblock++ , nYdata++ ){
			Yblock[jblock] = cmgem.ygen.first + (float) nYdata * cmgem.ygen.delta;
		    }
		    nYdata -= 2 ; /* theres a two point overlap between blocks. */
		}
		else{
		    jyy = jyb;
		    for( jblock = 1; jblock <= nblock; jblock++ ){
			Yblock[jblock] = Yarray[jyy];
			jyy = jyy + incy;
		    }
		}
	    } /* end else associated with if( lqdp ) */

	    /* --- Take logs of data if logarithmic interpolation is requested. */

	    if( cmgem.ixint == AXIS_LOG ){
		if( cmgem.floor > 0 )
		    logdta( xblock, nblock, cmgem.lfloor, cmgem.floor, xblock, nerr );
		else
		    logdta( xblock, nblock, cmgem.lfloor, cmgem.data.xmin, xblock, nerr );

		if( *nerr != 0 )
		    return ;
	    } /* end if( cmgem.ixint == AXIS_LOG ) */

	    if( cmgem.iyint == AXIS_LOG ){
		if( cmgem.floor > 0 )
		    logdta( yblock, nblock, cmgem.lfloor, cmgem.floor, yblock, nerr );
		else
		    logdta( yblock, nblock, cmgem.lfloor, cmgem.data.ymin, yblock, nerr );

		if( *nerr != 0 )
		    return ;
	    } /* end if( cmgem.iyint == AXIS_LOG ) */

	    /* --- Map the input data points into the plot coordinate system. */
	    for( j3 = 1; j3 <= nblock; j3++ ){
		/* following line added to support portrait mode in prs, maf 960829 */
		if( !(cmgem.lnull && (Xblock[j3] == cmgem.vnull)) )
		    Xblock[j3] = cmgem.xmpip1*Xblock[j3] + cmgem.xmpip2;
		if( !(cmgem.lnull && (Yblock[j3] == cmgem.vnull)) )
		    Yblock[j3] = cmgem.ympip1*Yblock[j3] + cmgem.ympip2;
	    }


      /* --- Plot the data with the fill color */
      if(cmgem.lfill) {
        if(cmgem.ifill[0] >= 0) { /* Positive */
          polyfill(xblock, yblock, nblock, TRUE, cmgem.ifill[0]);
        }
        if(cmgem.ifill[1] >= 0) { /* Negative */
          polyfill(xblock, yblock, nblock, FALSE, cmgem.ifill[1]);
        }
      }

	    /* --- Plot the data points (with or without clipping). */

	    setvspaceclip( lclip );
	    if( lclip || cmgem.lnull )
		plclip( xblock, yblock, nblock, lnewdp );
	    else
		plnocl( xblock, yblock, &nblock, lnewdp );

	    /* --- Update indices and counters for next block,
	     *     allowing for overlap of blocks. */

	    ncopyd = ncopyd + nblock - 1;
	    if( lqdp ){
		jxb = jxb - incx*inc;
		jyb = jyb - incy*inc;
	    }
	    else{
		if( !cmgem.xgen.on )
		    jxb = jxx - 2*incx;

		if( !cmgem.ygen.on )
		    jyb = jyy - 2*incy;

	    }

	    /* --- Each block after the first one is of size MBLOCK+2.
	     *     Extra two points allows for overlap when QDP is on. */

	    lnewdp = FALSE;
	    nblinbuf = nblinbuf + 1;
	    if( nblinbuf == 300 ){
		nblinbuf = 0;
	    }

	    nblock = MBLOCK + 2;
	} /* end for( jcopy ) */

	/* -- End of loop on data blocks. */

	/* - Reset various data display attributes to their "skeleton" values. */

	/* -- linestyle. */
	setlinestyle( LINE_STYLE_SOLID );

	/* -- color */
	if( cmgem.lcol ) {
          setcolor( cmgem.iskcol );
        } else {
          setcolor( color_foreground_default() );
        }

	/* -- line-width */

	setlinewidth( cmgem.iskwidth );

	/* - Increment various data display attributes if requested. */

	/* -- linestyle. */
	if( cmgem.lline && cmgem.liline )
	    incat( cmgem.icline, cmgem.iiline, cmgem.niline, &cmgem.jiline, 
	     &cmgem.icline );

	/* -- line color */
	if( cmgem.lcol && cmgem.licol )
	    incat( cmgem.icol, cmgem.iicol, cmgem.nicol, &cmgem.jicol, &cmgem.icol );
	/* -- line-width */
	if( cmgem.lwidth && cmgem.liwidth )
	    incat( cmgem.iwidth, cmgem.iiwidth, cmgem.niwidth, &cmgem.jiwidth, 
	     &cmgem.iwidth );

	/* -- symbol number. */
	if( cmgem.lsym && cmgem.lisym ){
	    incat( cmgem.isym, cmgem.iisym, cmgem.nisym, &cmgem.jisym, &insym );
	    cmgem.isym = insym;
	    if( cmgem.isym != 0 )
		setsymbolnum( cmgem.isym );
	}
  if( cmgem.lfill & cmgem.lifill ) {
    incat(cmgem.ifill[0], cmgem.iifillp, cmgem.nifill, &cmgem.jifill[0], &cmgem.ifill[0]);
    incat(cmgem.ifill[1], cmgem.iifilln, cmgem.nifill, &cmgem.jifill[1], &cmgem.ifill[1]);
  }

	/* - Write the desampling factor on the plot if the QDP option is on. */

	if( lqdp ){
	    cmgem.chht = cmgem.tsdef;
	    cmgem.chwid = cmgem.txrat*cmgem.chht;
	    settextsize( cmgem.chwid, cmgem.chht );
	    cnvita( inc/2, kinc,9 );
	    ljust( kinc,9 );
	    ninc = indexb( kinc,9 );
	    getstringsize( kinc, ninc, &slen );
	    xrectangle = slen + cmgem.chht;
	    yrectangle = 2.*cmgem.chht;
	    x1 = cmgem.uplot.xmax - xrectangle;
	    x2 = cmgem.uplot.xmax;
	    y1 = cmgem.uplot.ymin;
	    y2 = cmgem.uplot.ymin + yrectangle;
	    setlinewidth( cmgem.iskwidth );
	    if( (cmgem.iskwidth > 1) && cmgem.lwidth ){
		line( x2, y2, x1 - cmgem.iskwidth*skfudge, y2 );
		line( x1, y2 + cmgem.iskwidth*skfudge, x1, y1 );
	    }
	    else{
		line( x2, y2, x1, y2 );
		line( x1, y2, x1, y1 );
	    }
	    setlinewidth( LINE_WIDTH_THIN );
	    settextjust( "CENTER", "CENTER" );
	    pltext( kinc,9, x1 + 0.5*xrectangle, y1 + 0.5*yrectangle );
	    setlinewidth( cmgem.iskwidth );
	} /* end if ( lqdp ) */

} /* end of function */

