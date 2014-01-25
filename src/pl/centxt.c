
#include "pl.h"
#include "gem.h"
#include "co.h"
#include "gdm.h"

void /*FUNCTION*/ centxt(ktext, ktext_s, ntext, itextp, tsize)
char *ktext;   int ktext_s;
int ntext, itextp;
double tsize;
{
	float anglsv, slen, textx, texty, tssave, xpwid, ypwid;


	/* Ind
	 *=====================================================================
	 * PURPOSE: To center a text string relative to current plot.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    KTEXT:   Text to be centered.
	 *    NTEXT:   Number of characters in KTEXT.
	 *=====================================================================
	 * MODULE/LEVEL:  GEM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *=====================================================================
	 * GLOBAL COUPLING:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * ASSUMPTIONS:
	 *=====================================================================
	 * LIMITATIONS:
	 *=====================================================================
	 * KNOWN ERRORS:
	 *=====================================================================
	 * EXTERNAL DEPENDENCIES:
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Change to requested character size. */
	cmgem.chht = tsize;
	cmgem.chwid = cmgem.txrat*cmgem.chht;
	settextsize( cmgem.chwid, cmgem.chht );

	/* - Save character size in case we have to modify it. */

	/* ts = tsize; */
	tssave = cmgem.tscur;
	anglsv = 0.0;

	/* - Compute location of text to be centered on top of plot. */
	if( itextp == TOP ){

		/* -- Change to horizontal text orientation. */
		settextangle( TEXT_HORIZONTAL );

		/* -- Locate x starting location for text.
		 *    See if text will fit in the available space.
		 *    If not, decrease character size so that text will just fit. */
		xpwid = cmgem.uplot.xmax - cmgem.uplot.xmin;
		getstringsize( ktext, ntext, &slen );
		if( slen > xpwid ){
			/* ts = ts*xpwid/slen; */
			cmgem.chht = cmgem.tsdef;
			cmgem.chwid = cmgem.txrat*cmgem.chht;
			settextsize( cmgem.chwid, cmgem.chht );
			}
		textx = cmgem.uplot.xmin + 0.5*xpwid;

		/* -- Locate y starting location for text. */
		texty = fmin( cmgem.uplot.ymax + cmgem.axis[TOP].width + 0.75*cmgem.chht, 
		 cmgem.view.ymax - 0.6*cmgem.chht );
		cmgem.axis[TOP].width = texty + 0.5*cmgem.chht - cmgem.uplot.ymax;

		/* - Compute location of text to be centered at bottom of plot. */

		}
	else if( itextp == BOTTOM ){
		settextangle( TEXT_HORIZONTAL );
		xpwid = cmgem.uplot.xmax - cmgem.uplot.xmin;
		getstringsize( ktext, ntext, &slen );
		if( slen > xpwid ){
			/* ts = ts*xpwid/slen; */
			cmgem.chht = cmgem.tsdef;
			cmgem.chwid = cmgem.txrat*cmgem.chht;
			settextsize( cmgem.chwid, cmgem.chht );
			}
		textx = cmgem.uplot.xmin + 0.5*xpwid;
		texty = fmax( cmgem.uplot.ymin - cmgem.axis[BOTTOM].width - 0.75*cmgem.chht, 
		 cmgem.view.ymin + 0.6*cmgem.chht );
		cmgem.axis[BOTTOM].width = cmgem.uplot.ymin - texty + 0.5*cmgem.chht;

		/* - Compute location of text to be centered to the left of plot. */

		}
	else if( itextp == LEFT ){
		settextangle( TEXT_VERTICAL );
		ypwid = cmgem.uplot.ymax - cmgem.uplot.ymin;
		getstringsize( ktext, ntext, &slen );
		if( slen > ypwid ){
			/* ts = ts*ypwid/slen; */
			cmgem.chht = cmgem.tsdef;
			cmgem.chwid = cmgem.txrat*cmgem.chht;
			settextsize( cmgem.chwid, cmgem.chht );
			}
		textx = fmax( cmgem.uplot.xmin - cmgem.axis[LEFT].width - 0.75*cmgem.chht, 
		 cmgem.view.xmin + 0.6*cmgem.chht );
		texty = cmgem.uplot.ymin + 0.5*ypwid;
		cmgem.axis[LEFT].width = cmgem.uplot.xmin - textx + 0.5*cmgem.chht;

		/* - Compute location of text to be centered to the right of plot. */

		}
	else{
		settextangle( TEXT_VERTICAL );
		ypwid = cmgem.uplot.ymax - cmgem.uplot.ymin;
		getstringsize( ktext, ntext, &slen );
		if( slen > ypwid ){
			/* ts = ts*ypwid/slen; */
			cmgem.chht = cmgem.tsdef;
			cmgem.chwid = cmgem.txrat*cmgem.chht;
			settextsize( cmgem.chwid, cmgem.chht );
			}
		textx = fmin( cmgem.uplot.xmax + cmgem.axis[RIGHT].width + 0.75*cmgem.chht, 
		 cmgem.view.xmax - 0.6*cmgem.chht );
		texty = cmgem.uplot.ymin + 0.5*ypwid;
		cmgem.axis[RIGHT].width = textx - cmgem.uplot.xmax + 0.5*cmgem.chht;
		}

	/* - Write centered text at computed location. */

	settextjust( "CENTER", "CENTER" );
	pltext( ktext,ktext_s, textx, texty );

	/* - Restore character size and orientation attributes. */

	cmgem.chht = tssave;
	cmgem.chwid = cmgem.txrat*cmgem.chht;
	settextsize( cmgem.chwid, cmgem.chht );
	settextangle( anglsv );

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    841024:  Changed logic to use actual string length to center text.
	 *    831006:  Replaced calls to GETAXW/PUTAXW with inline coding.
	 *    821004:  Minor changes in text positions.
	 *    820331:  Major restructuring.
	 *===================================================================== */

} /* end of function */

