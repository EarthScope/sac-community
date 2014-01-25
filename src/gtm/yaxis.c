/** 
 * @file   yaxis.c
 * 
 * @brief  Create Y Axis
 * 
 */

#include <string.h>

#include "gtm.h"
#include "bool.h"


#include "gdm.h"
#include "bot.h"

void 
yaxis (char *type, 
       char *annot, 
       char *ticks, 
       char *label, 
       int label_s) {

	char ktype[3], kxjust[9], kyjust[9];
	int lleftax, lleftlb, llefttc, lrightax, lrighttc, lylin;
	char kannot, kticks;
	int nc;
	float hgtch, widch, widleft, widright, xlab, ylab;


	/*=====================================================================
	 * PURPOSE:  To produce a linear or logarithmically interpolated axis
	 *           at the left and/or right of the current viewport.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    type:    Type of interpolation to use. [c]
	 *             = 'LINEAR' for linear interpolation.
	 *             = 'LOGARITHMIC' for logarithmic interpolation.
	 *    annot:   Location of axis annotation. [c]
	 *             = 'LEFT' to place annotated axis at left of viewport.
	 *             = 'RIGHT' to place annotated axis at right of viewport.
	 *             = 'BOTH' to place annotated axis at right and left.
	 *             = 'NONE' for no axis annotation.
	 *    ticks:   Location of axis tick marks. [c]
	 *             = 'LEFT' to place tick marks at left of viewport.
	 *             = 'RIGHT' to place tick marks at right of viewport.
	 *             = 'BOTH' to place tick marks at right and left.
	 *             = 'NONE' for no tick marks.
	 *    label:   Text of label to place with annotation. [c]
	 *             Ignored if blank.
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    vsmall
	 *    gtm:     xvpmin, xvpmax, yvpmin, yvpmax
	 *=====================================================================
	 * GLOBAL COUPLING:
	 * - Use setworld and setvport to set world coordinates and viewport.
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  upcase, ylinaxis, ylogaxis, indexb, gettextsize,
	 *             settextangle, gettextjust, text, settextjust
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861027:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861027
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Convert first two characters of arguments to uppercase for testing. */
	upcase( type, 2, ktype,3 );
	upcase( annot, 1, &kannot, 1 );
	upcase( ticks, 1, &kticks, 1 );

	/* - Convert passed arguments to more useful logical flags. */

	if( strcmp(ktype,"LO") == 0 ){
		lylin = FALSE;
		}
	else{
		lylin = TRUE;
		}

	if( kannot == 'L' ){
		lrightax = FALSE;
		lleftax = TRUE;
		lleftlb = TRUE;
		}
	else if( kannot == 'R' ){
		lrightax = TRUE;
		lleftax = FALSE;
		lleftlb = FALSE;
		}
	else if( kannot == 'B' ){
		lrightax = TRUE;
		lleftax = TRUE;
		lleftlb = TRUE;
		}
	else{
		lrightax = FALSE;
		lleftax = FALSE;
		lleftlb = TRUE;
		}

	if( kticks == 'L' ){
		lrighttc = FALSE;
		llefttc = TRUE;
		}
	else if( kticks == 'R' ){
		lrighttc = TRUE;
		llefttc = FALSE;
		}
	else if( kticks == 'B' ){
		lrighttc = TRUE;
		llefttc = TRUE;
		}
	else{
		lrighttc = FALSE;
		llefttc = FALSE;
		}

	/* - Call linear or log axis maker. */

	if( lylin ){
		ylinaxis( lleftax, lrightax, llefttc, lrighttc, &widleft, 
		 &widright );
		}
	else{
		ylogaxis( lleftax, lrightax, llefttc, lrighttc, &widleft, 
		 &widright );
		}

	/* - Label axis if requested. */

	nc = indexb( label,label_s );
	if( nc > 0 ){
		ylab = 0.5*(cmgtm.yvpmin + cmgtm.yvpmax);
		gettextsize( &widch, &hgtch );
		if( lleftlb ){
			xlab = cmgtm.xvpmin - widleft - 0.75*hgtch;
			}
		else{
			xlab = cmgtm.xvpmax + widright + 0.75*hgtch;
			}
		settextangle( 90. );
		gettextjust( kxjust,9, kyjust, 9 );
		settextjust( "CENTER", "CENTER" );
		move( xlab, ylab );
		text( label,label_s, nc );
		settextangle( 0. );
		settextjust( kxjust, kyjust );
		}

       
	return;

} /* end of function */

