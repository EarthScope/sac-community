
#include <string.h>

#include "pl.h"
#include "gem.h"
#include "co.h"
#include "bool.h"


#include "gtm.h"
#include "bot.h"
#include "ucf.h"
#include "gdm.h"

#define	FSECAX	8.
#define	FSECTC	4.
#define	MDECLB	4

void /*FUNCTION*/ ylogax()
{
	char kdec[9];
	int lsecax, lsectc;
	int idecin, idecmn, idecmx, isecin, jdec, jfac, 
	 nc, ndivu;
	float decade, decmn, decmx, decsiz, skfudge, slen, slen10, 
	 slenmx, xloc, xvpmax, xvpmin, ypmnf, ypmxf, yref, yrefs, yvpmax, 
	 yvpmin;


	/*=====================================================================
	 * PURPOSE:  To produce a logarithmically-scaled axis to the right
	 *           and/or left of the current plot window.
	 *=====================================================================
	 * MODULE/LEVEL:  GEM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GEM:     YIMNZ, YIMXZ, YPMNU, YPMXU, LLOGLB,
	 *             'LEFT', 'CENTER', 'RIGHT', 'BOTTOM', 'TOP',
	 *             LRIGAX, LRIGTC, LLEFAX, LLEFTC,
	 *             YMPIP1, YMPIP2, LYGRD, FAC(),
	 *             ITHIN, ISKWIDTH, IWIDTH, SKDEVFUDGE
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GEM:     AXWRIG, AXWLEF
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  CNVITA, LJUST, LINE, PLTEXT, GETSTRINGSIZE
	 *             SETLINEWIDTH, GETVPORT
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    MDECLB:  Maximum number of labeled decades.
	 *    FSECAX:  Factor used to determine secondary labels.
	 *    FSECTC:  Factor used to determine secondary tick marks.
	 *    LSECAX:  .TRUE. if there is enough room for secondary axis labels.
	 *    LSECTC:  .TRUE. if there is enough room for secondary tick marks.
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Label every decade if there are no more than MDECLB of them. */
	decmn = cmgem.zdata.ymin;
	if( decmn <= 0. ){
		idecmn = decmn;
		}
	else{
		idecmn = decmn + 0.9999999;
		}
	decmx = cmgem.zdata.ymax;
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

	decsiz = fabs( cmgem.uplot.ymax - cmgem.uplot.ymin )/(cmgem.zdata.ymax - cmgem.zdata.ymin);
	isecin = 1;
	if( (decsiz > FSECAX*cmgem.chht && idecin == 1) && cmgem.lloglb ){
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
	for( jdec = idecmn; jdec <= idecmx; jdec += idecin ){
		cnvita( jdec, kdec,9 );
		ljust( kdec,9 );
		nc = indexb( kdec,9 );
		getstringsize( kdec, nc, &slen );
		slenmx = fmax( slenmx, slen );
		}
	getstringsize( "10", 2, &slen10 );

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

		/* -- Put secondary labels on decade before first full one, if necessary. */
		decade = (float)( idecmn - 1 );
		yref = decade*cmgem.ympip1 + cmgem.ympip2;
		ypmnf = cmgem.uplot.ymin - 0.0002;
		ypmxf = cmgem.uplot.ymax + 0.0002;
		for( jfac = 2; jfac <= 9; jfac++ ){
			yrefs = yref + cmgem.fac[jfac-1]*cmgem.ympip1;
			if( yrefs >= ypmnf && yrefs <= ypmxf ){
				if( (lsecax && cmgem.axis[LEFT].annotate) && (jfac%isecin) == 0 ){
					xloc = cmgem.uplot.xmin - 0.1*cmgem.chwid;
					settextjust( "RIGHT", "CENTER" );
					pltext( &kmgem.kfac[jfac-1],1, xloc, yrefs );
					setlinewidth( cmgem.iskwidth );
					}
				if( lsectc ){
					line( cmgem.uplot.xmin, yrefs, cmgem.uplot.xmin + 
					 0.5*cmgem.chwid, yrefs );
					}
				}
			}

		/* -- Put primary and secondary labels on remainder of axis. */
		strcpy( kdec, "        " );
		for( jdec = idecmn; jdec <= idecmx; jdec += idecin ){
			decade = (float)( jdec );
			cnvita( jdec, kdec,9 );
			ljust( kdec,9 );
			yref = decade*cmgem.ympip1 + cmgem.ympip2;
			if( cmgem.axis[LEFT].annotate ){
				xloc = cmgem.uplot.xmin - 0.2*cmgem.chwid - slenmx;
				settextjust( "RIGHT", "CENTER" );
				pltext( "10",3, xloc, yref );
				settextjust( "LEFT", "BOTTOM" );
				pltext( kdec,9, xloc, yref );
				setlinewidth( cmgem.iskwidth );
				}
			line( cmgem.uplot.xmin, yref, cmgem.uplot.xmin + cmgem.chwid, 
			 yref );
			for( jfac = 2; jfac <= 9; jfac++ ){
				yrefs = yref + cmgem.fac[jfac-1]*cmgem.ympip1;
				if( yrefs <= ypmxf ){
					if( (lsecax && cmgem.axis[LEFT].annotate) && (jfac%isecin) == 
					 0 ){
						xloc = cmgem.uplot.xmin - cmgem.chwid;
						settextjust( "LEFT", "CENTER" );
						pltext( &kmgem.kfac[jfac-1],1, xloc, yrefs );
						setlinewidth( cmgem.iskwidth );
						}
					if( lsectc ){
						line( cmgem.uplot.xmin, yrefs, cmgem.uplot.xmin + 
						 .5*cmgem.chwid, yrefs );
						}
					}
				}
			}

		/* -- Save axes widths. */
		if( cmgem.axis[LEFT].annotate ){
			cmgem.axis[LEFT].width = 0.2*cmgem.chwid + slenmx + slen10;
			}
		else{
			cmgem.axis[LEFT].width = 0.;
			}

		}

	/* - Right axis. */

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

		/* -- Put secondary labels on decade before first full one, if necessary. */
		decade = (float)( idecmn - 1 );
		yref = decade*cmgem.ympip1 + cmgem.ympip2;
		ypmnf = cmgem.uplot.ymin - 0.0002;
		ypmxf = cmgem.uplot.ymax + 0.0002;
		for( jfac = 2; jfac <= 9; jfac++ ){
			yrefs = yref + cmgem.fac[jfac-1]*cmgem.ympip1;
			if( yrefs >= ypmnf && yrefs <= ypmxf ){
				if( (lsecax && cmgem.axis[RIGHT].annotate) && (jfac%isecin) == 0 ){
					xloc = cmgem.uplot.xmax + 0.1*cmgem.chwid;
					settextjust( "LEFT", "CENTER" );
					pltext( &kmgem.kfac[jfac-1],1, xloc, yrefs );
					}
				if( lsectc ){
					line( cmgem.uplot.xmax, yrefs, cmgem.uplot.xmax - 
					 0.5*cmgem.chwid, yrefs );
					}
				}
			}

		/* -- Put primary and secondary labels on remainder of axis. */
		strcpy( kdec, "        " );
		for( jdec = idecmn; jdec <= idecmx; jdec += idecin ){
			decade = (float)( jdec );
			cnvita( jdec, kdec,9 );
			ljust( kdec,9 );
			yref = decade*cmgem.ympip1 + cmgem.ympip2;
			if( cmgem.axis[RIGHT].annotate ){
				xloc = cmgem.uplot.xmax + 0.2*cmgem.chwid + slen10;
				settextjust( "RIGHT", "CENTER" );
				pltext( "10",3, xloc, yref );
				settextjust( "LEFT", "BOTTOM" );
				pltext( kdec,9, xloc, yref );
				setlinewidth( cmgem.iskwidth );
				}
			line( cmgem.uplot.xmax, yref, cmgem.uplot.xmax - cmgem.chwid, 
			 yref );
			for( jfac = 2; jfac <= 9; jfac++ ){
				yrefs = yref + cmgem.fac[jfac-1]*cmgem.ympip1;
				if( yrefs <= ypmxf ){
					if( (lsecax && cmgem.axis[RIGHT].annotate) && (jfac%isecin) == 
					 0 ){
						xloc = cmgem.uplot.xmax + 0.1*cmgem.chwid;
						settextjust( "LEFT", "CENTER" );
						pltext( &kmgem.kfac[jfac-1],1, xloc, yrefs );
						setlinewidth( cmgem.iskwidth );
						}
					if( lsectc ){
						line( cmgem.uplot.xmax, yrefs, cmgem.uplot.xmax - 
						 .5*cmgem.chwid, yrefs );
						}
					if( cmgem.lygrd ){
						setlinestyle( cmgem.iygrd );
						setlinewidth( LINE_WIDTH_THIN );
						line( cmgem.uplot.xmin, yrefs, cmgem.uplot.xmax, 
						 yrefs );
						setlinestyle( LINE_STYLE_SOLID );
						setlinewidth( cmgem.iwidth );
						}
					}
				}
			}

		/* -- Save axes widths. */
		if( cmgem.axis[RIGHT].annotate ){
			cmgem.axis[RIGHT].width = 0.2*cmgem.chwid + slen10 + slenmx;
			}
		else{
			cmgem.axis[RIGHT].width = 0.;
			}

		}

	/* - Grid lines. */

	if( cmgem.lygrd ){
		setlinestyle( cmgem.iygrd );
		setlinewidth( LINE_WIDTH_THIN );
		decade = (float)( idecmn - 1 );
		yref = decade*cmgem.ympip1 + cmgem.ympip2;
		ypmnf = cmgem.uplot.ymin - 0.0002;
		ypmxf = cmgem.uplot.ymax + 0.0002;
		for( jfac = 2; jfac <= 9; jfac++ ){
			yrefs = yref + cmgem.fac[jfac-1]*cmgem.ympip1;
			if( yrefs > ypmnf && yrefs <= ypmxf ){
				line( cmgem.uplot.xmin, yrefs, cmgem.uplot.xmax, yrefs );
				}
			}
		for( jdec = idecmn; jdec <= idecmx; jdec += idecin ){ 
			decade = (float)( jdec );
			yref = decade*cmgem.ympip1 + cmgem.ympip2;
			line( cmgem.uplot.xmin, yref, cmgem.uplot.xmax, yref );

			for( jfac = 2; jfac <= 9; jfac++ ){
				yrefs = yref + cmgem.fac[jfac-1]*cmgem.ympip1;
				if( yrefs <= ypmxf )
					line( cmgem.uplot.xmin, yrefs, cmgem.uplot.xmax, yrefs );
				}
			}
		setlinestyle( LINE_STYLE_SOLID );
		setlinewidth( cmgem.iwidth );
		}

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    920530:  Added WDITH option. TEXT is always thin!
	 *    830927:  Moved grid drawing logic to its own do loop.
	 *    821001:  Cleaned up and documented.
	 *    810120:  Original version.
	 *===================================================================== */

} /* end of function */

