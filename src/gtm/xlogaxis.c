#include <string.h>
#include <math.h>

#include "gtm.h"
#include "gem.h"
#include "bool.h"
#include "co.h"

#include "gdm.h"
#include "bot.h"
#include "ucf.h"

#define	FSECAX	8.
#define	FSECTC	4.
#define	MDECLB	4

void xlogaxis(int lbotax, int ltopax, int lbottc, int ltoptc, float *widbot, 
              float *widtop)
{

	char kdec[9], khorz[9], kvert[9];
	int lsecax, lsectc;
	int idecin, idecmn, idecmx, iline, isecin, 
	 jdec, jfac, jfac_, nc, ndivu;
	float chht, chwid, decade, decmn, decmx, decsiz, slen, slen10, 
	 slenmx, xmpip2, xpmnf, xpmxf, xpmxu, xref, xrefs, yloc, ypmnu, 
	 ypmxu;

	/*=====================================================================
	 * *** INTERNAL SUBROUTINE:  NOT NORMALLY CALLED BY USER ***
	 *=====================================================================
	 * PURPOSE:  To produce a logarithmically-scaled axis at the bottom
	 *           and/or top of the current plot window.
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
	 *    mach:
	 *    gtm:     xwcmin, xwcmax, xvpmin, xvpmax,
	 *             xmpwv1, xmpip2, lxgrid, fac
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  cnvita, ljust, line, text, setlinestyle
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    mdeclb:  Maximum number of labeled decades.
	 *    fsecax:  Factor used to determine secondary labels.
	 *    fsectc:  Factor used to determine secondary tick marks.
	 *    lsecax:  .TRUE. if there is enough room for secondary axis labels.
	 *    lsectc:  .TRUE. if there is enough room for secondary tick marks.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861020:  Original version based upon XLOGAX.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861020
	 *===================================================================== */
	/* PROCEDURE: */
	ypmnu = 0.0;
	xmpip2 = 0.0;
	ypmxu = 0.0;
	xpmxu = 0.0;
	/* - Get current graphics character size. */
	gettextsize( &chwid, &chht );
	
	/* - Label every decade if there are no more than MDECLB of them. */

	decmn = cmgtm.xwcmin;
	if( decmn <= 0. ){
		idecmn = decmn;
		}
	else{
		idecmn = decmn + 0.9999999;
		}
	decmx = cmgtm.xwcmax;
	if( decmx >= 0. ){
		idecmx = decmx;
		}
	else{
		idecmx = decmx - 0.9999999;
		}
	ndivu = idecmx - idecmn + 1;
	idecin = ndivu/MDECLB;
	if( idecin < 1 )
		idecin = 1;

	/* - Determine if there is room for secondary labels and/or tick marks. */

	decsiz = fabs( cmgtm.xvpmax - cmgtm.xvpmin )/(cmgtm.xwcmax - cmgtm.xwcmin);
	if( (decsiz > FSECAX*chwid && idecin == 1) && cmgem.lloglb ){
		lsecax = TRUE;
		isecin = 2;
		if( decsiz > 2.*FSECAX*chwid )
			isecin = 1;
		}
	else{
		lsecax = FALSE;
		}
	if( decsiz > FSECTC*chwid && idecin == 1 ){
		lsectc = TRUE;
		}
	else{
		lsectc = FALSE;
		}

	/* - Calculate width of exponent. */

	slenmx = 0.;
	for( jdec = idecmn; jdec <= idecmx; jdec += idecin){
		cnvita( jdec, kdec,9 );
		ljust( kdec,9 );
		nc = indexb( kdec,9 );
		getstringsize( kdec, nc, &slen );
		slenmx = fmax( slenmx, slen );
		}
	getstringsize( "10", 2, &slen10 );

	/* - Save current linestyle and text justification. Set linestyle to solid. */

	getlinestyle( &iline );
	gettextjust( khorz,9, kvert,9 );
	setlinestyle( LINE_STYLE_SOLID );

	/* - Draw the bottom axes. */

	if( lbotax || lbottc ){

		/* -- Axes line. */
		line( cmgtm.xvpmin, ypmnu, cmgtm.xvpmax, ypmnu );

		/* -- Put secondary labels on decade before first full one, if necessary. */

		decade = (float)( idecmn - 1 );
		xref = decade*cmgtm.xmpwv1 + xmpip2;
		xpmnf = cmgtm.xvpmin - 0.0002;
		xpmxf = cmgtm.xvpmax + 0.0002;
		for( jfac = 2; jfac <= 9; jfac++ ){
			jfac_ = jfac - 1;
			xrefs = xref + cmgtm.fac[jfac_]*cmgtm.xmpwv1;
			if( xrefs >= xpmnf && xrefs <= xpmxf ){
				if( (lsecax && lbotax) && (jfac%isecin) == 0 ){
					yloc = cmgtm.yvpmin - 0.1*chht;
					settextjust( "CENTER", "TOP" );
					move( xrefs, yloc );
					text( &kmgtm.kfac[jfac_],1, 1 );
					}
				if( lsectc ){
					line( xrefs, cmgtm.yvpmin, xrefs, ypmnu + 0.5*chwid );
					}
				}
			}

		/* -- Put primary and secondary labels on remainder of axis. */
		strcpy( kdec, "        " );
		for( jdec = idecmn; jdec <= idecmx; jdec += idecin){
			decade = (float)( jdec );
			cnvita( jdec, kdec,9 );
			ljust( kdec,9 );
			xref = decade*cmgtm.xmpwv1 + xmpip2;
			if( lbotax ){
				yloc = cmgtm.yvpmin - 1.2*chht;
				settextjust( "RIGHT", "TOP" );
				move( xref, yloc );
				text( "10",3, 2 );
				settextjust( "LEFT", "CENTER" );
				move( xref, yloc );
				nc = indexb( kdec,9 );
				text( kdec,9, nc );
				}
			line( xref, cmgtm.yvpmin, xref, ypmnu + chwid );
			for( jfac = 2; jfac <= 9; jfac++ ){
				jfac_ = jfac - 1;
				xrefs = xref + cmgtm.fac[jfac_]*cmgtm.xmpwv1;
				if( xrefs <= xpmxf ){
					if( (lsecax && lbotax) && (jfac%isecin) == 0 ){
						yloc = cmgtm.yvpmin - 0.1*chht;
						settextjust( "CENTER", "TOP" );
						move( xrefs, yloc );
						text( &kmgtm.kfac[jfac_],1, 1 );
						}
					if( lsectc ){
						line( xrefs, cmgtm.yvpmin, xrefs, ypmnu + 0.5*chwid );
						}
					}
				}
			}

		/* -- Save axes widths. */
		if( lbotax ){
			*widbot = 2.2*chht;
			}
		else{
			*widbot = 0.;
			}

		}

	/* - Top axis. */

	if( ltopax || ltoptc ){

		/* -- Axes line. */
		line( cmgtm.xvpmin, cmgtm.yvpmax, xpmxu, ypmxu );

		/* -- Put secondary labels on decade before first full one, if necessary. */
		decade = (float)( idecmn - 1 );
		xref = decade*cmgtm.xmpwv1 + xmpip2;
		xpmnf = cmgtm.xvpmin - 0.0002;
		xpmxf = cmgtm.xvpmax + 0.0002;
		for( jfac = 2; jfac <= 9; jfac++ ){
			jfac_ = jfac - 1;
			xrefs = xref + cmgtm.fac[jfac_]*cmgtm.xmpwv1;
			if( xrefs >= xpmnf && xrefs <= xpmxf ){
				if( (lsecax && ltopax) && (jfac%isecin) == 0 ){
					yloc = cmgtm.yvpmax + 0.1*chht;
					settextjust( "CENTER", "BOTTOM" );
					move( xrefs, yloc );
					text( &kmgtm.kfac[jfac_],1, 1 );
					}
				if( lsectc ){
					line( xrefs, cmgtm.yvpmax, xrefs, ypmxu - 0.5*chwid );
					}
				}
			}

		/* -- Put primary and secondary labels on remainder of axis. */
		strcpy( kdec, "        " );
		for( jdec = idecmn; jdec <= idecmx; jdec += idecin){
			decade = (float)( jdec );
			cnvita( jdec, kdec,9 );
			ljust( kdec,9 );
			xref = decade*cmgtm.xmpwv1 + xmpip2;
			if( ltopax ){
				yloc = cmgtm.yvpmax + 1.2*chht;
				settextjust( "RIGHT", "CENTER" );
				move( xref, yloc );
				text( "10",3, 2 );
				settextjust( "LEFT", "BOTTOM" );
				nc = indexb( kdec,9 );
				text( kdec,9, nc );
				}
			line( xref, cmgtm.yvpmax, xref, ypmxu - chwid );
			for( jfac = 2; jfac <= 9; jfac++ ){
				jfac_ = jfac - 1;
				xrefs = xref + cmgtm.fac[jfac_]*cmgtm.xmpwv1;
				if( xrefs <= xpmxf ){
					if( (lsecax && ltopax) && (jfac%isecin) == 0 ){
						yloc = cmgtm.yvpmax + 0.1*chht;
						settextjust( "CENTER", "BOTTOM" );
						move( xrefs, yloc );
						text( &kmgtm.kfac[jfac_],1, 1 );
						}
					if( lsectc ){
						line( xrefs, cmgtm.yvpmax, xrefs, ypmxu - 0.5*chwid );
						}
					}
				}
			}

		/* -- Save axes widths. */
		if( ltopax ){
			*widtop = 2.2*chht;
			}
		else{
			*widtop = 0.;
			}

		}

	/* - Grid lines. */

	if( cmgem.lxgrd ){
		setlinestyle( cmgem.ixgrd );
		decade = (float)( idecmn - 1 );
		xref = decade*cmgtm.xmpwv1 + xmpip2;
		xpmnf = cmgtm.xvpmin - 0.0002;
		xpmxf = cmgtm.xvpmax + 0.0002;
		for( jfac = 2; jfac <= 9; jfac++ ){
			jfac_ = jfac - 1;
			xrefs = xref + cmgtm.fac[jfac_]*cmgtm.xmpwv1;
			if( xrefs > xpmnf && xrefs <= xpmxf ){
				line( xrefs, cmgtm.yvpmin, xrefs, cmgtm.yvpmax );
				}
			}
		for( jdec = idecmn; jdec <= idecmx; jdec += idecin){
			decade = (float)( jdec );
			xref = decade*cmgtm.xmpwv1 + xmpip2;
			line( xref, cmgtm.yvpmin, xref, cmgtm.yvpmax );
			for( jfac = 2; jfac <= 9; jfac++ ){
				jfac_ = jfac - 1;
				xrefs = xref + cmgtm.fac[jfac_]*cmgtm.xmpwv1;
				if( xrefs <= xpmxf )
					line( xrefs, cmgtm.yvpmin, xrefs, cmgtm.yvpmax );
				}
			}
		}

	/* - Restore linestyle and text justification. */

	setlinestyle( iline );
	settextjust( khorz, kvert );

       
	return;

} /* end of function */

