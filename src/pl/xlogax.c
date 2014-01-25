
#include <string.h>

#include "pl.h"
#include "gem.h"
#include "bool.h"
#include "co.h"


#include "gdm.h"
#include "bot.h"
#include "ucf.h"
#include "gtm.h"

#define	FSECAX	8.
#define	FSECTC	4.
#define	MDECLB	4

void /*FUNCTION*/ xlogax()
{
	char kdec[9];
	int lsecax, lsectc;
	int idecin, idecmn, idecmx, isecin, jdec, jfac, 
	 nc, ndivu;
	float decade, decmn, decmx, decsiz, skfudge, slen, slen10, 
	 slenmx, xpmnf, xpmxf, xref, xrefs, xvpmax, xvpmin, yloc, yvpmax, 
	 yvpmin;


	/*=====================================================================
	 * PURPOSE:  To produce a logarithmically-scaled axis at the bottom
	 *           and/or top of the current plot window.
	 *=====================================================================
	 * MODULE/LEVEL:  GEM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GEM:     XIMNZ, XIMXZ, XPMNU, XPMXU, LLOGLB,
	 *             'LEFT', 'CENTER', 'RIGHT', 'BOTTOM', 'TOP',
	 *             LBOTAX, LBOTTC, LTOPAX, LTOPTC,
	 *             XMPIP1, XMPIP2, LXGRD, ISOLID, IDOT, FAC(),
	 *             ISKWIDTH, ITHIN, IWIDTH, SKDEVFUDGE
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GEM:     AXWBOT, AXWTOP
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  CNVITA, LJUST, LINE, PLTEXT, SETLINESTYLE, SETLINEWIDTH
	 *             GETVPORT
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    MDECLB:  Maximum number of labeled decades.
	 *    FSECAX:  Factor used to determine secondary labels.
	 *    FSECTC:  Factor used to determine secondary tick marks.
	 *    LSECAX:  .TRUE. if there is enough room for secondary axis labels.
	 *    LSECTC:  .TRUE. if there is enough room for secondary tick marks.
	 *    xskfudge:Fudge factor when skeleton line-width gt 1. [f]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920526:  Added line-width. TEXT is always thin!
	 *    830927:  Moved grid drawing logic into its own do loop.
	 *    820930:  Cleanup up and documented.
	 *    810120:  Original PRIME version.
	 *===================================================================== */
	/* PROCEDURE: */

        settextangle(TEXT_HORIZONTAL);

	/* - Label every decade if there are no more than MDECLB of them. */
	decmn = cmgem.zdata.xmin;
	if( decmn <= 0. ){
		idecmn = decmn;
		}
	else{
		idecmn = decmn + 0.999;
		}
	decmx = cmgem.zdata.xmax;
	if( decmx >= 0. ){
		idecmx = decmx;
		}
	else{
		idecmx = decmx - 0.999;
		}
	ndivu = idecmx - idecmn + 1;
	idecin = ndivu/MDECLB;
	if( idecin < 1 )
		idecin = 1;

	/* - Determine if there is room for secondary labels and/or tick marks. */

	decsiz = fabs( cmgem.uplot.xmax - cmgem.uplot.xmin )/(cmgem.zdata.xmax - cmgem.zdata.xmin);
	isecin = 1;
	if( (decsiz > FSECAX*cmgem.chwid && idecin == 1) && cmgem.lloglb ){
		lsecax = TRUE;
		isecin = 2;
		if( decsiz > 2.*FSECAX*cmgem.chht )
			isecin = 1;
		}
	else{
		lsecax = FALSE;
		}
	if( decsiz > FSECTC*cmgem.chht && idecin == 1 ){
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

	/* - Determine axes fudge factor for thick axes lines. */
	getvport( &xvpmin, &xvpmax, &yvpmin, &yvpmax );
	skfudge = cmgem.skdevfudge*((yvpmin - yvpmax)/(xvpmin - xvpmax));

	/* - Draw the bottom axes. */

	setlinestyle( LINE_STYLE_SOLID );
	setlinewidth( cmgem.iskwidth );

	if( cmgem.axis[BOTTOM].annotate || cmgem.axis[BOTTOM].ticks ){

		/* -- Botton Axes line. */
		if( cmgem.iskwidth > LINE_WIDTH_THIN ){
			line( cmgem.uplot.xmin - cmgem.iskwidth*skfudge, cmgem.uplot.ymin, 
			 cmgem.uplot.xmax + cmgem.iskwidth*skfudge, cmgem.uplot.ymin );
			}
		else{
			line( cmgem.uplot.xmin, cmgem.uplot.ymin, cmgem.uplot.xmax, cmgem.uplot.ymin );
			}

		/* -- Put secondary labels on decade before first full one, if necessary. */

		decade = (float)( idecmn - 1 );
		xref = decade*cmgem.xmpip1 + cmgem.xmpip2;
		xpmnf = cmgem.uplot.xmin - 0.0002;
		xpmxf = cmgem.uplot.xmax + 0.0002;
		for( jfac = 2; jfac <= 9; jfac++ ){
			xrefs = xref + cmgem.fac[jfac-1]*cmgem.xmpip1;
			if( xrefs >= xpmnf && xrefs <= xpmxf ){
				if( (lsecax && cmgem.axis[BOTTOM].annotate) && (jfac%isecin) == 0 ){
					yloc = cmgem.uplot.ymin - 0.1*cmgem.chht;
					settextjust( "CENTER", "TOP" );
					pltext( &kmgem.kfac[jfac-1],1, xrefs, yloc );
					setlinewidth( cmgem.iskwidth );
					}
				if( lsectc ){
					line( xrefs, cmgem.uplot.ymin, xrefs, cmgem.uplot.ymin + 
					 0.5*cmgem.chwid );
					}
				}
			}

		/* -- Put primary and secondary labels on remainder of axis. */
		strcpy( kdec, "        " );
		for( jdec = idecmn; jdec <= idecmx; jdec += idecin ){
			decade = (float)( jdec );
			cnvita( jdec, kdec,9 );
			ljust( kdec,9 );
			xref = decade*cmgem.xmpip1 + cmgem.xmpip2;
			if( cmgem.axis[BOTTOM].annotate ){
				yloc = cmgem.uplot.ymin - 1.2*cmgem.chht;
				settextjust( "RIGHT", "TOP" );
				pltext( "10",3, xref, yloc );
				settextjust( "LEFT", "CENTER" );
				pltext( kdec,9, xref, yloc );
				setlinewidth( cmgem.iskwidth );
				}
			line( xref, cmgem.uplot.ymin, xref, cmgem.uplot.ymin + 
			 cmgem.chwid );
			for( jfac = 2; jfac <= 9; jfac++ ){
				xrefs = xref + cmgem.fac[jfac-1]*cmgem.xmpip1;
				if( xrefs <= xpmxf ){
					if( (lsecax && cmgem.axis[BOTTOM].annotate) && (jfac%isecin) == 
					 0 ){
						yloc = cmgem.uplot.ymin - 0.1*cmgem.chht;
						settextjust( "CENTER", "TOP" );
						pltext( &kmgem.kfac[jfac-1],1, xrefs, yloc );
						setlinewidth( cmgem.iskwidth );
						}
					if( lsectc ){
						line( xrefs, cmgem.uplot.ymin, xrefs, cmgem.uplot.ymin + 
						 0.5*cmgem.chwid );
						}
					}
				}
			}

		/* -- Save axes widths. */
		if( cmgem.axis[BOTTOM].annotate ){
			cmgem.axis[BOTTOM].width = 2.2*cmgem.chht;
			}
		else{
			cmgem.axis[BOTTOM].width = 0.;
			}

		}

	/* - Top axis. */

	if( cmgem.axis[TOP].annotate || cmgem.axis[TOP].ticks ){

		/* -- Top Axes line. */
		if( cmgem.iskwidth > LINE_WIDTH_THIN ){
			line( cmgem.uplot.xmin - cmgem.iskwidth*skfudge, cmgem.uplot.ymax, 
			 cmgem.uplot.xmax + cmgem.iskwidth*skfudge, cmgem.uplot.ymax );
			}
		else{
			line( cmgem.uplot.xmin, cmgem.uplot.ymax, cmgem.uplot.xmax, cmgem.uplot.ymax );
			}

		/* -- Put secondary labels on decade before first full one, if necessary. */
		decade = (float)( idecmn - 1 );
		xref = decade*cmgem.xmpip1 + cmgem.xmpip2;
		xpmnf = cmgem.uplot.xmin - 0.0002;
		xpmxf = cmgem.uplot.xmax + 0.0002;
		for( jfac = 2; jfac <= 9; jfac++ ){
			xrefs = xref + cmgem.fac[jfac-1]*cmgem.xmpip1;
			if( xrefs >= xpmnf && xrefs <= xpmxf ){
				if( (lsecax && cmgem.axis[TOP].annotate) && (jfac%isecin) == 0 ){
					yloc = cmgem.uplot.ymax + 0.1*cmgem.chht;
					settextjust( "CENTER", "BOTTOM" );
					pltext( &kmgem.kfac[jfac-1],1, xrefs, yloc );
					setlinewidth( cmgem.iskwidth );
					}
				if( lsectc ){
					line( xrefs, cmgem.uplot.ymax, xrefs, cmgem.uplot.ymax - 
					 0.5*cmgem.chwid );
					}
				}
			}

		/* -- Put primary and secondary labels on remainder of axis. */
		strcpy( kdec, "        " );
		for( jdec = idecmn; jdec <= idecmx; jdec += idecin ){
			decade = (float)( jdec );
			cnvita( jdec, kdec,9 );
			ljust( kdec,9 );
			xref = decade*cmgem.xmpip1 + cmgem.xmpip2;
			if( cmgem.axis[TOP].annotate ){
				yloc = cmgem.uplot.ymax + 1.2*cmgem.chht;
				settextjust( "RIGHT", "CENTER" );
				pltext( "10",3, xref, yloc );
				settextjust( "LEFT", "BOTTOM" );
				pltext( kdec,9, xref, yloc );
				setlinewidth( cmgem.iskwidth );
				}
			line( xref, cmgem.uplot.ymax, xref, cmgem.uplot.ymax - 
			 cmgem.chwid );
			for( jfac = 2; jfac <= 9; jfac++ ){
				xrefs = xref + cmgem.fac[jfac-1]*cmgem.xmpip1;
				if( xrefs <= xpmxf ){
					if( (lsecax && cmgem.axis[TOP].annotate) && (jfac%isecin) == 
					 0 ){
						yloc = cmgem.uplot.ymax + 0.1*cmgem.chht;
						settextjust( "CENTER", "BOTTOM" );
						pltext( &kmgem.kfac[jfac-1],1, xrefs, yloc );
						setlinewidth( cmgem.iskwidth );
						}
					if( lsectc ){
						line( xrefs, cmgem.uplot.ymax, xrefs, cmgem.uplot.ymax - 
						 0.5*cmgem.chwid );
						}
					}
				}
			}

		/* -- Save axes widths. */
		if( cmgem.axis[TOP].annotate ){
			cmgem.axis[TOP].width = 2.2*cmgem.chht;
			}
		else{
			cmgem.axis[TOP].width = 0.;
			}

		}

	/* - Grid lines. */

	if( cmgem.lxgrd ){
		setlinestyle( cmgem.ixgrd );
		setlinewidth( LINE_WIDTH_THIN );
		decade = (float)( idecmn - 1 );
		xref = decade*cmgem.xmpip1 + cmgem.xmpip2;
		xpmnf = cmgem.uplot.xmin - 0.0002;
		xpmxf = cmgem.uplot.xmax + 0.0002;
		for( jfac = 2; jfac <= 9; jfac++ ){
			xrefs = xref + cmgem.fac[jfac-1]*cmgem.xmpip1;
			if( xrefs > xpmnf && xrefs <= xpmxf ){
				line( xrefs, cmgem.uplot.ymin, xrefs, cmgem.uplot.ymax );
				}
			}
		for( jdec = idecmn; jdec <= idecmx; jdec += idecin ){
			decade = (float)( jdec );
			xref = decade*cmgem.xmpip1 + cmgem.xmpip2;
			line( xref, cmgem.uplot.ymin, xref, cmgem.uplot.ymax );
			for( jfac = 2; jfac <= 9; jfac++ ){
				xrefs = xref + cmgem.fac[jfac-1]*cmgem.xmpip1;
				if( xrefs <= xpmxf )
					line( xrefs, cmgem.uplot.ymin, xrefs, cmgem.uplot.ymax );
				}
			}
		setlinestyle( LINE_STYLE_SOLID );
		setlinewidth( cmgem.iwidth );
		}

       
	return;

} /* end of function */

