
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

void /*FUNCTION*/ xlinaxis(lbotax, ltopax, lbottc, ltoptc, widbot, 
	 widtop)
int lbotax, ltopax, lbottc, ltoptc;
float *widbot, *widtop;
{
	char khorz[9], ktemp[9], kvert[9];
	int lpower, lxpowr;
	int ia, ib, igdlog, iline, jdiv, jpower, jstep, 
	 jtick, mds, nc, nds, ndsu, ntick, nxdivu;
	float chht, chwid, divlog, divtry, factor, grdlog, power, 
	 value, valuei, xdivu, xgrdmn, xgrdmx, xref, xrefi, xtick, xticki, 
	 yloc, ypow, yvspmx;
	static char kvalue[17] = "                ";
	static char kpower[9] = "        ";

	kvert[ 0 ] = '\0' ;

	/*=====================================================================
	 * *** INTERNAL SUBROUTINE:  NOT NORMALLY CALLED BY USER ***
	 *=====================================================================
	 * PURPOSE:  To make a linearly interpolated x axis.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    lbotax:  Set to .TRUE. for an annotated axis at bottom of
	 *             the current viewport. [l]
	 *    ltopax:  Set to .TRUE. for an annotated axis at the top. [l]
	 *    lbottc:  Set to .TRUE. for tick marks at the bottom. [l]
	 *    ltoptc:  Set to .TRUE. for tick marks at the top. [l]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    widbot:  Viewport width of axes annotation at bottom of plot. [f]
	 *    widtop:  Viewport width of axes annotation at top of plot. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *   gtm:      lxdiv, xdiv, lnxdiv, nxdiv, xnicsp, lxgrid, ixgrid
	 *             xvpmin, xvpmax, yvpmin, yvpmax, xwcmin, xwcmax,
	 *             xmpwv1, xmpwv2
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  gettextsize, cnvita, ljust, getlinestyle, gettestjust
	 *             setlinestyle, line, move, text, cnvfta
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    xdivu:   Divison spacing used.
	 *    lpower:  Set to .TRUE. if there is a multiplying scale factor.
	 *    power:   Multiplying scale factor.
	 *    kpower:  Character string containing formatted scale factor.
	 *    ypow:    Y location in plot coordinates of scale factor.
	 *    divtry:  Trial division spacing.
	 *    jstep:   Integer trial step size (constrained to be 10, 5 or 2).
	 *    xgrdmn:  Minimum labeled grid value (including scale factor).
	 *    xgrdmx:  Maximum labeled grid value (including scale factor).
	 *    value:   Labeled grid value excluding scale factor.
	 *    valuei:  Increment in VALUE.
	 *    kvalue:  Character string containing formatted label value.
	 *    xref:    Location of labeled grid value in plot coordinates.
	 *    xrefi:   Increment in XREF.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861028:  Original version based upon XLINAX.
	 *=====================================================================
	 * DOCUMENTED:  861028
	 *===================================================================== */
	lxpowr = 0;
	yvspmx = 0.0;
	/* - Get current graphics text size to use in computations. */
	gettextsize( &chwid, &chht );

	/* - Determine division spacing.  There are three possibilities:
	 *   (1) The division spacing is set by user (LXDIV=.TRUE.).
	 *   (2) The (approximate) number of divsions is set (LNXDIV=.TRUE.).
	 *   (3) "Nice" division spacings are calculated. */

	if( cmgtm.lxdiv ){
		xdivu = cmgtm.xdiv;
		power = log10( xdivu );
		if( power < 0. )
			power = power - 1.;
		jpower = power;
		}
	else{
		if( cmgtm.lnxdiv ){
			nxdivu = cmgtm.nxdiv;
			}
		else{
			nxdivu = (cmgtm.xvpmax - cmgtm.xvpmin)/(cmgtm.xnicsp*chht);
			if( nxdivu < 5 )
				nxdivu = 5;
			}
		divtry = (cmgtm.xwcmax - cmgtm.xwcmin)/nxdivu;
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
		xdivu = jstep*(powi(10.,jpower));
		}

	/* - Determine "nice-numbered" starting and ending values. */

	ia = cmgtm.xwcmin/xdivu;
	xgrdmn = xdivu*ia;
	if( xgrdmn < cmgtm.xwcmin ){
		ia = ia + 1;
		xgrdmn = xgrdmn + xdivu;
		}
	ib = cmgtm.xwcmax/xdivu;
	xgrdmx = xdivu*ib;
	if( xgrdmx > cmgtm.xwcmax ){
		ib = ib - 1;
		xgrdmx = xgrdmx - xdivu;
		}
	nxdivu = ib - ia + 1;

	/* - Determine the format (Fn.m) of the labels.
	 *   The variable NDS assumes the role of "n" and MDS the role of "m". */

	/* - The "magic numbers" used in this algorithm  generate good division
	 *   spacings almost all of the time.  Modify them at your own risk. */

	grdlog = log10( fmax( fabs( xgrdmn ), fabs( xgrdmx ) ) + 0.001 );
	if( grdlog >= 0. ){
		grdlog = grdlog + 1.001;
		}
	else{
		grdlog = grdlog - 0.999;
		}
	divlog = log10( xdivu );
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
		if( labs( igdlog ) >= 3 && lxpowr ){
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

	/* - Draw the bottom axis. */

	if( lbotax || lbottc ){

		/* -- Axes line. */
		line( cmgtm.xvpmin, cmgtm.yvpmin, cmgtm.xvpmax, cmgtm.yvpmin );

		/* -- Label for multiplying scale factor. */
		if( lpower && lbotax ){
			ypow = fmax( cmgtm.yvpmin - 2.2*chht, 0.1*chht );
			settextjust( "LEFT", "BOTTOM" );
			move( cmgtm.xvpmin, ypow );
			nc = indexb( kpower,9 );
			text( kpower,9, nc );
			}

		/* -- Calculate constants for labeled tick marks. */
		value = xgrdmn*factor;
		xref = xgrdmn*cmgtm.xmpwv1 + cmgtm.xmpwv2;
		valuei = xdivu*factor;
		xrefi = xdivu*cmgtm.xmpwv1;
		strcpy( kvalue, "                " );

		/* -- Draw secondary tick marks before first labeled one. */
		ntick = 1;
		if( xrefi >= 0.10 ){
			ntick = 3;
			if( jstep == 5 )
				ntick = 4;
			}
		if( xrefi >= 0.25 )
			ntick = 9;
		xticki = xrefi/(float)( ntick + 1 );
		xtick = xref - xrefi;
		for( jtick = 1; jtick <= ntick; jtick++ ){
			xtick = xtick + xticki;
			if( xtick >= cmgtm.xvpmin ){
				line( xtick, cmgtm.yvpmin, xtick, cmgtm.yvpmin + 0.5*chwid );
				}
			}

		/* -- Loop on labeled tick marks. */
		for( jdiv = 1; jdiv <= nxdivu; jdiv++ ){
			line( xref, cmgtm.yvpmin, xref, cmgtm.yvpmin + chwid );
			if( lbotax ){
				if( value >= 0 ){
					ndsu = nds;
					}
				else{
					ndsu = nds + 1;
					}
				cnvfta( value, ndsu, mds, kvalue,17 );
				ljust( kvalue,17 );
				yloc = cmgtm.yvpmin - 0.1*chht;
				settextjust( "CENTER", "TOP" );
				move( xref, yloc );
				nc = indexb( kvalue,17 );
				text( kvalue,17, nc );
				}
			/* --- Loop on secondary tick marks. */
			xtick = xref;
			for( jtick = 1; jtick <= ntick; jtick++ ){
				xtick = xtick + xticki;
				if( xtick <= cmgtm.xvpmax ){
					line( xtick, cmgtm.yvpmin, xtick, cmgtm.yvpmin + 0.5*chwid );
					}
				}
			value = value + valuei;
			xref = xref + xrefi;
			}

		/* -- Save axes widths. */
		if( lbotax ){
			*widbot = 1.1*chht;
			if( lpower )
				*widbot = cmgtm.yvpmin - ypow;
			}
		else{
			*widbot = 0.;
			}

		}

	/* - Top axis: */

	if( ltopax || ltoptc ){

		/* -- Axes line. */
		line( cmgtm.xvpmin, cmgtm.yvpmax, cmgtm.xvpmax, cmgtm.yvpmax );

		/* -- Label for multiplying scale factor. */
		if( lpower && ltopax ){
			ypow = fmin( cmgtm.yvpmax + 2.2*chht, yvspmx - 0.1*chht );
			settextjust( "LEFT", "TOP" );
			move( cmgtm.xvpmin, ypow );
			nc = indexb( kpower,9 );
			text( kpower,9, nc );
			}

		/* -- Calculate constants for labeled tick marks. */
		value = xgrdmn*factor;
		xref = xgrdmn*cmgtm.xmpwv1 + cmgtm.xmpwv2;
		valuei = xdivu*factor;
		xrefi = xdivu*cmgtm.xmpwv1;
		strcpy( kvalue, "                " );

		/* -- Draw secondary tick marks before first labeled one. */
		ntick = 1;
		if( xrefi >= 0.10 ){
			ntick = 3;
			if( jstep == 5 )
				ntick = 4;
			}
		if( xrefi >= 0.25 )
			ntick = 9;
		xticki = xrefi/(float)( ntick + 1 );
		xtick = xref - xrefi;
		for( jtick = 1; jtick <= ntick; jtick++ ){
			xtick = xtick + xticki;
			if( xtick >= cmgtm.xvpmin ){
				line( xtick, cmgtm.yvpmax, xtick, cmgtm.yvpmax - 0.5*chwid );
				}
			}

		/* -- Loop on labeled tick marks. */
		for( jdiv = 1; jdiv <= nxdivu; jdiv++ ){
			line( xref, cmgtm.yvpmax, xref, cmgtm.yvpmax - chwid );
			if( ltopax ){
				if( value >= 0. ){
					ndsu = nds;
					}
				else{
					ndsu = nds + 1;
					}
				cnvfta( value, ndsu, mds, kvalue,17 );
				ljust( kvalue,17 );
				yloc = cmgtm.yvpmax + 0.1*chht;
				settextjust( "CENTER", "BOTTOM" );
				move( xref, yloc );
				nc = indexb( kvalue,17 );
				text( kvalue,17, nc );
				}
			/* --- Loop on secondary tick marks. */
			xtick = xref;
			for( jtick = 1; jtick <= ntick; jtick++ ){
				xtick = xtick + xticki;
				if( xtick <= cmgtm.xvpmax ){
					line( xtick, cmgtm.yvpmax, xtick, cmgtm.yvpmax - 0.5*chwid );
					}
				}
			value = value + valuei;
			xref = xref + xrefi;
			}

		/* -- Save axes widths. */
		if( ltopax ){
			*widtop = 1.1*chht;
			if( lpower )
				*widtop = ypow - cmgtm.yvpmax;
			}
		else{
			*widtop = 0.;
			}

		}

	/* - Grid lines. */

	if( cmgem.lxgrd ){
		xref = xgrdmn*cmgtm.xmpwv1 + cmgtm.xmpwv2;
		xrefi = xdivu*cmgtm.xmpwv1;
		setlinestyle( cmgem.ixgrd );
		for( jdiv = 1; jdiv <= nxdivu; jdiv++ ){
			line( xref, cmgtm.yvpmin, xref, cmgtm.yvpmax );
			xref = xref + xrefi;
			}
		}

	/* - Restore linestyle and text justification attributes. */

	setlinestyle( iline );
	settextjust( khorz, kvert );

       

	return;

} /* end of function */

