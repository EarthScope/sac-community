
#include <string.h>

#include "gtm.h"
#include "bool.h"


#include "gdm.h"
#include "bot.h"

void /*FUNCTION*/ xaxis(type, annot, ticks, label, label_s)
char *type, *annot, *ticks, *label;   int label_s;
{
	char kannot[3], kticks[3], ktype[3], kxjust[9], kyjust[9];
	int lbotax, lbotlb, lbottc, ltopax, ltoptc, lxlin;
	int nc;
	float hgtch, widbot, widch, widtop, xlab, ylab;


	/*=====================================================================
	 * PURPOSE:  To produce a linear or logarithmically interpolated axis
	 *           at the top and/or bottom of the current viewport.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    type:    Type of interpolation to use. [c]
	 *             = 'LINEAR' for linear interpolation.
	 *             = 'LOGARITHMIC' for logarithmic interpolation.
	 *    annot:   Location of axis annotation. [c]
	 *             = 'BELOW' to place annotated axis below viewport.
	 *             = 'ABOVE' to place annotated axis above viewport.
	 *             = 'BOTH' to place annotated axis above and below.
	 *             = 'NONE' for no axis annotation.
	 *    ticks:   Location of axis tick marks. [c]
	 *             = 'BELOW' to place tick marks below viewport.
	 *             = 'ABOVE' to place tick marks above viewport.
	 *             = 'BOTH' to place tick marks above and below.
	 *             = 'NONE' for no tick marks.
	 *    label:   Text of label to place with annotation. [c]
	 *             Ignored if blank.
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    vsmall
	 *    gtm:     xvpmin, xvpmax
	 *=====================================================================
	 * GLOBAL COUPLING:
	 * - Use setworld and setvport to set world coordinates and viewport.
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  upcase, xlinaxis, xlogaxis, indexb, gettextsize,
	 *             gettextjust, text, settextjust
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861027:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861027
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Convert first two characters of arguments to uppercase for testing. */
	upcase( type, 2, ktype,3 );
	upcase( annot, 2, kannot,3 );
	upcase( ticks, 2, kticks,3 );

	/* - Convert passed arguments to more useful logical flags. */

	if( strcmp(ktype,"LO") == 0 ){
		lxlin = FALSE;
		}
	else{
		lxlin = TRUE;
		}

	if( strcmp(kannot,"AB") == 0 ){
		ltopax = TRUE;
		lbotax = FALSE;
		lbotlb = FALSE;
		}
	else if( strcmp(kannot,"BE") == 0 ){
		ltopax = FALSE;
		lbotax = TRUE;
		lbotlb = TRUE;
		}
	else if( strcmp(kannot,"BO") == 0 ){
		ltopax = TRUE;
		lbotax = TRUE;
		lbotlb = TRUE;
		}
	else{
		ltopax = FALSE;
		lbotax = FALSE;
		lbotlb = TRUE;
		}

	if( strcmp(kticks,"AB") == 0 ){
		ltoptc = TRUE;
		lbottc = FALSE;
		}
	else if( strcmp(kticks,"BE") == 0 ){
		ltoptc = FALSE;
		lbottc = TRUE;
		}
	else if( strcmp(kticks,"BO") == 0 ){
		ltoptc = TRUE;
		lbottc = TRUE;
		}
	else{
		ltoptc = FALSE;
		lbottc = FALSE;
		}

	/* - Call linear or log axis maker. */

	if( lxlin ){
		xlinaxis( lbotax, ltopax, lbottc, ltoptc, &widbot, &widtop );
		}
	else{
		xlogaxis( lbotax, ltopax, lbottc, ltoptc, &widbot, &widtop );
		}

	/* - Label axis if requested. */

	nc = indexb( label,label_s );
	if( nc > 0 ){
		xlab = 0.5*(cmgtm.xvpmin + cmgtm.xvpmax);
		gettextsize( &widch, &hgtch );
		if( lbotlb ){
			ylab = cmgtm.yvpmin - widbot - 0.75*hgtch;
			}
		else{
			ylab = cmgtm.yvpmax + widtop + 0.75*hgtch;
			}
		gettextjust( kxjust,9, kyjust,9 );
		settextjust( "CENTER", "CENTER" );
		move( xlab, ylab );
		text( label,label_s, nc );
		settextjust( kxjust, kyjust );
		}

       
	return;

} /* end of function */

