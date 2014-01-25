
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "pl.h"
#include "gem.h"
#include "co.h"
#include "bool.h"
#include "proto.h"
#include "gdm.h"


#include "gtm.h"
#include "bot.h"
#include "ucf.h"
#include "gdm.h"

#define	FDIVSP	5.

void /*FUNCTION*/ ylinax()
{
	char ktemp[9];
	int lneglb, lpower;
	int ia, ib, igdlog, jdiv, jpower, jstep, jtick, 
	 mds, nc, nds, ndsu, ntick, nydivu;
	float divlog, divtry, factor, grdlog, power, skfudge, 
	 slen, slenmx, value, valuei, xloc, xpow, xvpmax, xvpmin, ydivu, 
	 ygrdmn, ygrdmx, yref, yrefi, ytick, yticki, yvpmax, yvpmin;
	static char kvalue[17] = "                ";
	static char kpower[9] = "        ";


	/*=====================================================================
	 * PURPOSE:  To produce a linearly-scaled axis to the right and/or
	 *           left of the current plot window.
	 *=====================================================================
	 * MODULE/LEVEL:  GEM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GEM:     LYDIV, YDIV, LNYDIV, NYDIV, CHHT, CHWID,
	 *             YPMNU, YPMXU, YIMNZ, YIMXZ,
	 *             LLEFAX, LLEFTC, LRIGAX, LRIGTC,
	 *             IHORZ, IVERT, YMPIP1, YMPIP2,
	 *             LYGRD, IYGRD, ISOLID, IDOT
	 *             ISKWIDTH, ITHIN, IWIDTH, SKDEVFUDGE
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GEM:     AXWLEF, AXWRIG
	 *=====================================================================
	 * GLOBAL COUPLING:
	 * - CENTXT uses AXWLEF and AXWRIG to position y axis label.
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  CNVITA, LJUST, ZCNCAT, LINE, SETTEXTANGLE, PLTEXT, CNVFTA
	 *             SETLINEWIDTH, GETVPORT
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    YDIVU:   Divison spacing used.
	 *    LPOWER:  Set to .TRUE. if there is a multiplying scale factor.
	 *    POWER:   Multiplying scale factor.
	 *    KPOWER:  Character string containing formatted scale factor.
	 *    DIVTRY:  Trial division spacing.
	 *    JSTEP:   Integer trial step size (constrained to be 10, 5 or 2).
	 *    YGRDMN:  Minimum labeled grid value (including scale factor).
	 *    YGRDMX:  Maximum labeled grid value (including scale factor).
	 *    VALUE:   Labeled grid value excluding scale factor.
	 *    VALUEI:  Increment in VALUE.
	 *    KVALUE:  Character string containing formatted label value.
	 *    YREF:    Location of labeled grid value in plot coordinates.
	 *    YREFI:   Increment in YREF.
	 *    LNEGLB:  .TRUE. if some of the labels on the right axis are
	 *             negative.  Used to horizontally align labels. [l]
	 *=====================================================================
	 * ASSUMPTIONS:
	 * - PLMAP has set up world to plot coordiate mapping.
	 * - Text orientation is horizontal.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    970819:  Terminated kvalue  before padding.  maf
	 *    920526:  Added line-width. TEXT is always thin!
	 *    860422:  Added logic to properly align right axis labels.
	 *    830929:  Added secondary tick marks.
	 *    830927:  Moved grid drawing logic into its own do loop.
	 *    830223:  Fixed logic in computing annotation format.
	 *    820928:  Cleaned up and documented.
	 *    810120:  Original PRIME version.
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Determine division spacing.  There are three possibilities:
	 *   (1) The division spacing is set by user (LYDIV=.TRUE.).
	 *   (2) The (approximate) number of divsions is set (LNYDIV=.TRUE.).
	 *   (3) "Nice" division spacings are calculated. */

	if( cmgem.ydiv_spacing_on ){
		ydivu = cmgem.ydiv_spacing;
		power = log10( ydivu );
		if( power < 0. )
			power = power - 1.;
		jpower = power;
	}
	else{
		if( cmgem.ydiv_number_on ){
			nydivu = cmgem.ydiv_number;
		}
		else{
			nydivu = (cmgem.uplot.ymax - cmgem.uplot.ymin)/(FDIVSP*cmgem.chht);
			if( nydivu < 5 )
				nydivu = 5;
		}
		divtry = (cmgem.zdata.ymax - cmgem.zdata.ymin)/nydivu;
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

	ia = cmgem.zdata.ymin/ydivu;
	ygrdmn = ydivu*ia;
	if( ygrdmn < cmgem.zdata.ymin ){
		ia = ia + 1;
		ygrdmn = ygrdmn + ydivu;
	}
	ib = cmgem.zdata.ymax/ydivu;
	ygrdmx = ydivu*ib;
	if( ygrdmx > cmgem.zdata.ymax ){
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
		if( labs( igdlog ) >= 3 && cmgem.lypowr ){
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

	/* - Determine the axes fudge factor for thick axes lines. */
	getvport( &xvpmin, &xvpmax, &yvpmin, &yvpmax );
	skfudge = cmgem.skdevfudge*((yvpmin - yvpmax)/(xvpmin - xvpmax));

	/* - Draw the left axis. */

	setlinestyle( LINE_STYLE_SOLID );
	setlinewidth( cmgem.iskwidth );

	if( cmgem.axis[LEFT].annotate || cmgem.axis[LEFT].ticks ){

		/* -- Left Axes line. */
		if( cmgem.iskwidth > LINE_WIDTH_THIN ){
			line( cmgem.uplot.xmin, cmgem.uplot.ymin - cmgem.iskwidth*
			 skfudge, cmgem.uplot.xmin, cmgem.uplot.ymax + cmgem.iskwidth*
			 skfudge );
		}
		else{
			line( cmgem.uplot.xmin, cmgem.uplot.ymin, cmgem.uplot.xmin, cmgem.uplot.ymax );
		}

		/* -- Calculate constants for labeled tick marks. */
		settextangle( TEXT_HORIZONTAL );
		value = ygrdmn*factor;
		yref = ygrdmn*cmgem.ympip1 + cmgem.ympip2;
		valuei = ydivu*factor;
		yrefi = ydivu*cmgem.ympip1;
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
			if( ytick >= cmgem.uplot.ymin ){
				line( cmgem.uplot.xmin, ytick, cmgem.uplot.xmin + 
				 0.5*cmgem.chwid, ytick );
			}
		}

		/* -- Loop on labeled tick marks. */
		slenmx = 0.;
		for( jdiv = 1; jdiv <= nydivu; jdiv++ ){
			line( cmgem.uplot.xmin, yref, cmgem.uplot.xmin + cmgem.chwid, yref );
			if( cmgem.axis[LEFT].annotate ){
				if( value >= 0. ){
					ndsu = nds;
				}
				else{
					ndsu = nds + 1;
				}
				cnvfta( value, ndsu, mds, kvalue,17 );
				ljust( kvalue,17 );
				nc = indexb( kvalue,ndsu );	/* changed 17 to ndsu. maf 970819 */
				if ( kvalue[ nc ] == ' ' )	/* terminate where padding ... */
				    kvalue[nc] = '\0' ;		/* ... begins. maf 970819 */
				getstringsize( kvalue, nc, &slen );
				slenmx = fmax( slenmx, slen );
				xloc = cmgem.uplot.xmin - 0.1*cmgem.chwid;
				settextjust( "RIGHT", "CENTER" );
				pltext( kvalue,17, xloc, yref );
				setlinewidth( cmgem.iskwidth );
			}
			/* --- Loop on secondary tick marks. */
			ytick = yref;
			for( jtick = 1; jtick <= ntick; jtick++ ){
				ytick = ytick + yticki;
				if( ytick <= cmgem.uplot.ymax ){
					line( cmgem.uplot.xmin, ytick, cmgem.uplot.xmin + 
					 0.5*cmgem.chwid, ytick );
				}
			}
			value = value + valuei;
			yref = yref + yrefi;
		}

		/* -- Label for multiplying scale factor. */
		if( lpower && cmgem.axis[LEFT].annotate ){
			settextangle( TEXT_VERTICAL );
			/* if( lneg ){ */
			/* 	ndsu = nds + 1; */
			/* } */
			/* else{ */
			/* 	ndsu = nds; */
			/* } */
			xpow = fmax( cmgem.uplot.xmin - slenmx - 1.2*cmgem.chht, 0.1*
			 cmgem.chht );
			settextjust( "LEFT", "TOP" );
			pltext( kpower,9, xpow, cmgem.uplot.ymin );
			setlinewidth( cmgem.iskwidth );
		}

		/* -- Save axes widths. */
		if( cmgem.axis[LEFT].annotate ){
			cmgem.axis[LEFT].width = slenmx + 0.1*cmgem.chwid;
			if( lpower )
				cmgem.axis[LEFT].width = cmgem.uplot.xmin - xpow;
		}
		else{
			cmgem.axis[LEFT].width = 0.;
		}

	}

	/* - Now draw the right axis. */

	if( cmgem.axis[RIGHT].annotate || cmgem.axis[RIGHT].ticks ){

		/* -- Right Axes line. */
		if( cmgem.iskwidth > LINE_WIDTH_THIN ){
			line( cmgem.uplot.xmax, cmgem.uplot.ymin - cmgem.iskwidth*
			 skfudge, cmgem.uplot.xmax, cmgem.uplot.ymax + cmgem.iskwidth*
			 skfudge );
		}
		else{
			line( cmgem.uplot.xmax, cmgem.uplot.ymin, cmgem.uplot.xmax, cmgem.uplot.ymax );
		}

		/* -- Constants for labeled tick marks. */
		settextangle( TEXT_HORIZONTAL );
		value = ygrdmn*factor;
		yref = ygrdmn*cmgem.ympip1 + cmgem.ympip2;
		valuei = ydivu*factor;
		yrefi = ydivu*cmgem.ympip1;
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
			if( ytick >= cmgem.uplot.ymin ){
				line( cmgem.uplot.xmax, ytick, cmgem.uplot.xmax - 
				 0.5*cmgem.chwid, ytick );
			}
		}

		/* -- Loop on labeled tick marks. */
		slenmx = 0.;
		lneglb = FALSE;
		for( jdiv = 1; jdiv <= nydivu; jdiv++ ){
			line( cmgem.uplot.xmax, yref, cmgem.uplot.xmax - cmgem.chwid, 
			 yref );
			if( cmgem.axis[RIGHT].annotate ){
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
				xloc = cmgem.uplot.xmax + 0.1*cmgem.chwid;
				if( lneglb && value >= 0. )
					xloc = xloc + cmgem.chwid;
				settextjust( "LEFT", "CENTER" );
				pltext( kvalue,17, xloc, yref );
				setlinewidth( cmgem.iskwidth );
			}
			/* --- Loop on secondary tick marks. */
			ytick = yref;
			for( jtick = 1; jtick <= ntick; jtick++ ){
				ytick = ytick + yticki;
				if( ytick <= cmgem.uplot.ymax ){
					line( cmgem.uplot.xmax, ytick, cmgem.uplot.xmax - 
					 0.5*cmgem.chwid, ytick );
				}
			}
			value = value + valuei;
			yref = yref + yrefi;
		}

		/* -- Label for multiplying scale factor. */
		if( lpower && cmgem.axis[RIGHT].annotate ){
			settextangle( TEXT_VERTICAL );
			xpow = fmin( cmgem.uplot.xmax + slenmx + 1.2*cmgem.chwid, cmgem.view.xmax - 
			 0.1*cmgem.chht );
			settextjust( "LEFT", "BOTTOM" );
			pltext( kpower,9, xpow, cmgem.uplot.ymin );
			setlinewidth( cmgem.iskwidth );
			settextangle( TEXT_HORIZONTAL );
		}

		/* -- Save axes widths. */
		if( cmgem.axis[RIGHT].annotate ){
			cmgem.axis[RIGHT].width = slenmx + 0.1*cmgem.chwid;
			if( lpower )
				cmgem.axis[RIGHT].width = xpow - cmgem.uplot.xmax;
		}
		else{
			cmgem.axis[RIGHT].width = 0.;
		}

	}

	/* - Grid lines. */

	if( cmgem.lygrd ){
		yref = ygrdmn*cmgem.ympip1 + cmgem.ympip2;
		yrefi = ydivu*cmgem.ympip1;
		setlinestyle( cmgem.iygrd );
		for( jdiv = 1; jdiv <= nydivu; jdiv++ ){
			line( cmgem.uplot.xmin, yref, cmgem.uplot.xmax, yref );
			yref = yref + yrefi;
		}
		setlinestyle( LINE_STYLE_SOLID );
		setlinewidth( cmgem.iwidth );
	}

       

	return;

} /* end of function */

