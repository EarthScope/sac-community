
#include "pl.h"
#include "gem.h"


#include "gtm.h"
#include "gdm.h"

void /*FUNCTION*/ plgrid(nerr)
int *nerr;
{

	/*=====================================================================
	 * PURPOSE:  To draw grid lines, x and y axes and labels, and title.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred. [i]
	 *             Potential error numbers:  NONE
	 *=====================================================================
	 * MODULE/LEVEL:  GEM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GEM:     LBDR, TSAXIS, ILIN, ILOG, TSDEF
	 *             IXINT, LXLAB, KXLAB, NXLAB, IXLABP, TSXLAB,
	 *             IYINT, LYLAB, KYLAB, NYLAB, IYLABP, TSYLAB,
	 *             LTITL, KTITL, NTITL, ITITLP, TSTITL
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  RECTANGLE, SETTEXTSIZE, XLINAX, XLOGAX, CENTXT, YLINAX, YLOGAX, PL
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Draw border around plot window if requested. */

	if( cmgem.lbdr )
		rectangle( &cmgem.uplot.xmin, &cmgem.uplot.xmax, 
                           &cmgem.uplot.ymin, &cmgem.uplot.ymax );

	/* - X axis annotation. */

	cmgem.chht = cmgem.tsaxis;
	cmgem.chwid = cmgem.txrat*cmgem.chht;
	settextsize( cmgem.chwid, cmgem.chht );
	if( cmgem.ixint == AXIS_LINEAR ){
		xlinax();
		}
	else if( cmgem.ixint == AXIS_LOG ){
		xlogax();
		}
	/* - Y axis annotation. */

	if( cmgem.iyint == AXIS_LINEAR ){
		ylinax();
		}
	else if( cmgem.iyint == AXIS_LOG ){
		ylogax();
		}

	/* - Axes labels and title. */

	if( cmgem.ylabel.on )
		centxt( kmgem.kylab,145, cmgem.ylabel.len, cmgem.ylabel.pos, cmgem.ylabel.text_size );
	if( cmgem.xlabel.on )
		centxt( kmgem.kxlab,145, cmgem.xlabel.len, cmgem.xlabel.pos, cmgem.xlabel.text_size );
	if( cmgem.title.on )
		centxt( kmgem.ktitl,145, cmgem.title.len, cmgem.title.pos, cmgem.title.text_size );

	/* - General plot labels. */

	plplab();



	/* - Reset default text size. */

	cmgem.chht = cmgem.tsdef;
	cmgem.chwid = cmgem.txrat*cmgem.chht;
	settextsize( cmgem.chwid, cmgem.chht );

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    860605:  Fixed bug involving text size of y axis annotation.
	 *    830209:  Added call to PLPLAB.
	 *    811228:  Deleted calls to ZCLIP.
	 *===================================================================== */

} /* end of function */

