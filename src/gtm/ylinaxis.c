
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "gtm.h"
#include "gem.h"
#include "co.h"
#include "bool.h"
#include "proto.h"


#include "gdm.h"
#include "bot.h"
#include "ucf.h"

void /*FUNCTION*/ ylinaxis(llefax, lrigax, lleftc, lrigtc, widlef, 
	 widrig)
int llefax, lrigax, lleftc, lrigtc;
float *widlef, *widrig;
{
	char khorz[9], ktemp[9], kvert[9];
	int lneglb, lpower, lrev, lypowr;
	int ia, ib, igdlog, iline, jdiv, jpower, jstep, 
	 jtick, mds, nc, nds, ndsu, ntick, nydivu;
	float chht, chwid, divlog, divtry, factor, grdlog, power, 
	 slen, slenmx, value, valuei, xloc, xpow, xvspmx, ydivu, ygrdmn, 
	 ygrdmx, ymax, ymin, yref, yrefi, ytick, yticki;
	static char kvalue[17] = "                ";
	static char kpower[9] = "        ";


	/*=====================================================================
	 * *** INTERNAL SUBROUTINE:  NOT NORMALLY CALLED BY USER ***
	 *=====================================================================
	 * PURPOSE:  To make a linearly interpolated y axis.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    llefax:  Set to .TRUE. for an annotated axis to the left of
	 *             the current viewport. [l]
	 *    lrigax:  Set to .TRUE. for an annotated axis to the right. [l]
	 *    lleftc:  Set to .TRUE. for tick marks to the left. [l]
	 *    lrigtc:  Set to .TRUE. for tick marks to the right. [l]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    widlef:  Viewport width of annotated axes at left of viewport. [f]
	 *    widrig:  Viewport width of annotated axis at right of viewport. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    gtm:     lydiv, ydiv, lnydiv, nydiv, ynicsp, lygrid, iygrid
	 *             xvpmin, xvpmax, yvpmin, yvpmax, ywcmin, ywcmax,
	 *             ympwv1, ympwv2,
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  gettextsize, cnvita, ljust, indexb, getlinestyle,
	 *             setlinestyle, gettextjust, settextjust, line,
	 *             settextangle, getstringsize
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    ydivu:   Divison spacing used.
	 *    lpower:  Set to .TRUE. if there is a multiplying scale factor.
	 *    power:   Multiplying scale factor.
	 *    kpower:  Character string containing formatted scale factor.
	 *    divtry:  Trial division spacing.
	 *    jstep:   Integer trial step size (constrained to be 10, 5 or 2).
	 *    ygrdmn:  Minimum labeled grid value (including scale factor).
	 *    ygrdmx:  Maximum labeled grid value (including scale factor).
	 *    value:   Labeled grid value excluding scale factor.
	 *    valuei:  Increment in value.
	 *    kvalue:  Character string containing formatted label value.
	 *    yref:    Location of labeled grid value in plot coordinates.
	 *    yrefi:   Increment in yref.
	 *    lneglb:  .TRUE. if some of the labels on the right axis are
	 *             negative.  Used to horizontally align labels. [l]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861028:  Original version based upon YLINAX.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861028
	 *===================================================================== */
	/* PROCEDURE: */
	lypowr = 0;
	xvspmx = 0.0;
	/* - Get current graphics text size to use in computations. */
	gettextsize( &chwid, &chht );

	/* - Determine division spacing.  There are three possibilities:
	 *   (1) The division spacing is set by user (LYDIV=.TRUE.).
	 *   (2) The (approximate) number of divsions is set (LNYDIV=.TRUE.).
	 *   (3) "Nice" division spacings are calculated. */

	if( cmgtm.lydiv ){
		ydivu = cmgtm.ydiv;
		power = log10( ydivu );
		if( power < 0. )
			power = power - 1.;
		jpower = power;
		}
	else{
		if( cmgtm.lnydiv ){
			nydivu = cmgtm.nydiv;
			}
		else{
			nydivu = (cmgtm.yvpmax - cmgtm.yvpmin)/(cmgtm.ynicsp*chht);
			if( nydivu < 5 )
				nydivu = 5;
			}
		if( cmgtm.ywcmax < cmgtm.ywcmin ){
			lrev = TRUE;
			ymax = cmgtm.ywcmin;
			ymin = cmgtm.ywcmax;
			}
		else{
			lrev = FALSE;
			ymin = cmgtm.ywcmin;
			ymax = cmgtm.ywcmax;
			}
		divtry = (ymax - ymin)/nydivu;
		if( divtry > 0. ){
			power = log10( divtry );
			}
		else{
			power = 0.;
			}
		if( power < 0. )
			power = power - 1.;
		jpower = power;

		/* -- Limit divison spacings to steps of 10, 5, or 2 [cases (2) and (3)]. */
		jstep = divtry*(powi(10.,-jpower));
		if( jstep > 5 ){
			jstep = 1;
			jpower = jpower + 1;
			/* power = power + 1.; */
			}
		else if( jstep > 2 ){
			jstep = 5;
			}
		else{
			jstep = 2;
			}
		ydivu = (float)( jstep )*(powi(10.,jpower));
		}

	/* - Determine "nice-numbered" starting and ending values. */

	ia = ymin/ydivu;
	ygrdmn = ydivu*ia;
	if( ygrdmn < ymin ){
		ia = ia + 1;
		ygrdmn = ygrdmn + ydivu;
		}
	ib = ymax/ydivu;
	ygrdmx = ydivu*ib;
	if( ygrdmx > ymax ){
		ib = ib - 1;
		ygrdmx = ygrdmx - ydivu;
		}
	nydivu = ib - ia + 1;

	/* - Determine the format (Fn.m) of the labels.
	 *   The variable NDS assumes the role of "n" and MDS the role of "m". */

	/* - The "magic numbers" used in this algorithm  generate good division
	 *   spacings almost all of the time.  Modify them at your own risk. */

	grdlog = log10( fmax( fabs( ygrdmn ), fabs( ygrdmx ) ) + 0.001 );
	if( grdlog >= 0. ){
		grdlog = grdlog + 1.001;
		}
	else{
		grdlog = grdlog - 0.999;
		}
	divlog = log10( ydivu );
	if( divlog >= 0. ){
		divlog = divlog + 1.001;
		}
	else{
		divlog = divlog - 0.999;
		}
	lpower = FALSE;
	factor = 1.;
	if( grdlog*divlog >= 0. ){
		if( grdlog < 0. ){
			igdlog = grdlog;
			}
		else{
			igdlog = divlog;
			}
		if( labs( igdlog ) >= 3 && lypowr ){
			mds = 0;
			nds = max( 4, (int)( grdlog ) - (int)( divlog ) + 2 );
			cnvita( jpower, ktemp,9 );
			ljust( ktemp,9 );
			if( jpower >= 0 ){
                                fstrncpy( kpower, 8, "X 10+", 5);
                                fstrncpy( kpower+5, 8-5, ktemp, strlen(ktemp));
				}
			else{
                                fstrncpy( kpower, 8, "X 10", 4);
                                fstrncpy( kpower+4, 8-4, ktemp, strlen(ktemp));
				}
			factor = powi(10.,-jpower);
			lpower = TRUE;
			}
		else{
			mds = labs( minfi( 0., divlog ) );
			nds = maxfi( 1., grdlog );
			if( mds > 0 )
				nds = nds + mds + 2;
			}
		}
	else{
		mds = labs( minfi( 0., divlog ) );
		nds = maxfi( 0., grdlog );
		if( mds > 0 )
			nds = nds + mds + 2;
		}

	/* - Save current linestyle and text justification. Set linestyle to solid. */

	getlinestyle( &iline );
	gettextjust( khorz,9, kvert, 9 );
	setlinestyle( 1 );

	/* - Draw the left axis. */

	if( llefax || lleftc ){

		/* -- Axes line. */
		line( cmgtm.xvpmin, cmgtm.yvpmin, cmgtm.xvpmin, cmgtm.yvpmax );

		/* -- Calculate constants for labeled tick marks. */
		settextangle( 0. );
		value = ygrdmn*factor;
		yref = ygrdmn*cmgtm.ympwv1 + cmgtm.ympwv2;
		valuei = ydivu*factor;
		yrefi = ydivu*cmgtm.ympwv1;
		strcpy( kvalue, "                " );
		/* lneg = ygrdmn < 0. || ygrdmx < 0.; */

		/* -- Draw secondary tick marks before first labeled one. */
		ntick = 1;
		if( yrefi >= 0.10 ){
			ntick = 3;
			if( jstep == 5 )
				ntick = 4;
			}
		if( yrefi >= 0.25 )
			ntick = 9;
		yticki = yrefi/(float)( ntick + 1 );
		ytick = yref - yrefi;
		for( jtick = 1; jtick <= ntick; jtick++ ){
			ytick = ytick + yticki;
			if( lrev ){
				if( ytick < cmgtm.yvpmax ){
					line( cmgtm.xvpmin, ytick, cmgtm.xvpmin + 
					 0.5*chwid, ytick );
					}
				}
			else{
				if( ytick >= cmgtm.yvpmin ){
					line( cmgtm.xvpmin, ytick, cmgtm.xvpmin + 
					 0.5*chwid, ytick );
					}
				}
			}

		/* -- Loop on labeled tick marks. */
		slenmx = 0.;
		for( jdiv = 1; jdiv <= nydivu; jdiv++ ){
			line( cmgtm.xvpmin, yref, cmgtm.xvpmin + chwid, 
			 yref );
			if( llefax ){
				if( value >= 0. ){
					ndsu = nds;
					}
				else{
					ndsu = nds + 1;
					}
				cnvfta( value, ndsu, mds, kvalue,17 );
				ljust( kvalue,17 );
				nc = indexb( kvalue,17 );
				getstringsize( kvalue, nc, &slen );
				slenmx = fmax( slenmx, slen );
				xloc = cmgtm.xvpmin - 0.1*chwid;
				settextjust( "RIGHT", "CENTER" );
				move( xloc, yref );
				text( kvalue,17, nc );
				}
			/* --- Loop on secondary tick marks. */
			ytick = yref;
			for( jtick = 1; jtick <= ntick; jtick++ ){
				ytick = ytick + yticki;
				if( lrev ){
					if( ytick > cmgtm.yvpmin ){
						line( cmgtm.xvpmin, ytick, cmgtm.xvpmin + 
						 0.5*chwid, ytick );
						}
					}
				else{
					if( ytick <= cmgtm.yvpmax ){
						line( cmgtm.xvpmin, ytick, cmgtm.xvpmin + 
						 0.5*chwid, ytick );
						}
					}
				}
			value = value + valuei;
			yref = yref + yrefi;
			}

		/* -- Label for multiplying scale factor. */
		if( lpower && llefax ){
			settextangle( 90. );
			/* if( lneg ){ */
			/* 	ndsu = nds + 1; */
			/* 	} */
			/* else{ */
			/* 	ndsu = nds; */
			/* 	} */
			xpow = fmax( cmgtm.xvpmin - slenmx - 1.2*chht, 0.1*chht );
			settextjust( "LEFT", "TOP" );
			move( xpow, cmgtm.yvpmin );
			text( kpower,9, nc );
			settextangle( 0. );
			}

		/* -- Save axes widths. */
		if( llefax ){
			*widlef = slenmx + 0.1*chwid;
			if( lpower )
				*widlef = cmgtm.xvpmin - xpow;
			}
		else{
			*widlef = 0.;
			}

		}

	/* - Now draw the right axis. */

	if( lrigax || lrigtc ){

		/* -- Axes line. */
		line( cmgtm.xvpmax, cmgtm.yvpmin, cmgtm.xvpmax, cmgtm.yvpmax );

		/* -- Constants for labeled tick marks. */
		settextangle( 0. );
		value = ygrdmn*factor;
		yref = ygrdmn*cmgtm.ympwv1 + cmgtm.ympwv2;
		valuei = ydivu*factor;
		yrefi = ydivu*cmgtm.ympwv1;
		strcpy( kvalue, "                " );
		/* lneg = ygrdmn < 0. || ygrdmx < 0.; */

		/* -- Draw secondary tick marks before first labeled one. */
		ntick = 1;
		if( yrefi >= 0.10 ){
			ntick = 3;
			if( jstep == 5 )
				ntick = 4;
			}
		if( yrefi >= 0.25 )
			ntick = 9;
		yticki = yrefi/(float)( ntick + 1 );
		ytick = yref - yrefi;
		for( jtick = 1; jtick <= ntick; jtick++ ){
			ytick = ytick + yticki;
			if( lrev ){
				if( ytick < cmgtm.yvpmax ){
					line( cmgtm.xvpmax, ytick, cmgtm.xvpmax - 
					 0.5*chwid, ytick );
					}
				}
			else{
				if( ytick >= cmgtm.yvpmin ){
					line( cmgtm.xvpmax, ytick, cmgtm.xvpmax - 
					 0.5*chwid, ytick );
					}
				}
			}

		/* -- Loop on labeled tick marks. */
		slenmx = 0.;
		lneglb = FALSE;
		for( jdiv = 1; jdiv <= nydivu; jdiv++ ){
			line( cmgtm.xvpmax, yref, cmgtm.xvpmax - chwid, 
			 yref );
			if( lrigax ){
				if( value >= 0. ){
					ndsu = nds;
					}
				else{
					ndsu = nds + 1;
					lneglb = TRUE;
					}
				cnvfta( value, ndsu, mds, kvalue,17 );
				ljust( kvalue,17 );
				nc = indexb( kvalue,17 );
				getstringsize( kvalue, nc, &slen );
				slenmx = fmax( slenmx, slen );
				xloc = cmgtm.xvpmax + 0.1*chwid;
				if( lneglb && value >= 0. )
					xloc = xloc + chwid;
				settextjust( "LEFT", "CENTER" );
				move( xloc, yref );
				text( kvalue,17, nc );
				}
			/* --- Loop on secondary tick marks. */
			ytick = yref;
			for( jtick = 1; jtick <= ntick; jtick++ ){
				ytick = ytick + yticki;
				if( lrev ){
					if( ytick > cmgtm.yvpmin ){
						line( cmgtm.xvpmax, ytick, cmgtm.xvpmax - 
						 0.5*chwid, ytick );
						}
					}
				else{
					if( ytick <= cmgtm.yvpmax ){
						line( cmgtm.xvpmax, ytick, cmgtm.xvpmax - 
						 0.5*chwid, ytick );
						}
					}
				}
			value = value + valuei;
			yref = yref + yrefi;
			}

		/* -- Label for multiplying scale factor. */
		if( lpower && lrigax ){
			settextangle( 90. );
			xpow = fmin( cmgtm.xvpmax + slenmx + 1.2*chwid, xvspmx - 
			 0.1*chht );
			settextjust( "LEFT", "BOTTOM" );
			move( xpow, cmgtm.yvpmin );
			text( kpower,9, nc );
			settextangle( 0. );
			}

		/* -- Save axes widths. */
		if( lrigax ){
			*widrig = slenmx + 0.1*chwid;
			if( lpower )
				*widrig = xpow - cmgtm.xvpmax;
			}
		else{
			*widrig = 0.;
			}

		}

	/* - Grid lines. */

	if( cmgem.lygrd ){
        fprintf(stderr, "%f %f\n", cmgtm.ympwv1, cmgtm.ympwv2);
		yref = ygrdmn*cmgtm.ympwv1 + cmgtm.ympwv2;
		yrefi = ydivu*cmgtm.ympwv1;
		setlinestyle( cmgem.iygrd );
		for( jdiv = 1; jdiv <= nydivu; jdiv++ ){
			line( cmgtm.xvpmin, yref, cmgtm.xvpmax, yref );
			yref = yref + yrefi;
			}
		}

	/* - Restore linestyle and text justification attributes. */

	setlinestyle( iline );
	settextjust( khorz, kvert );

       

	return;

} /* end of function */

