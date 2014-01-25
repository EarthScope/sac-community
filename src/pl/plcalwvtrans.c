
#include "pl.h"
#include "gem.h"

void /*FUNCTION*/ plcalwvtrans()
{



	/*=====================================================================
	 * *** INTERNAL SUBROUTINE: NOT NORMALLY CALLED BY USER ***
	 *=====================================================================
	 * PURPOSE:  To calculate the world (input) to viewport (plot)
	 *           coordinate mapping transformation.
	 *=====================================================================
	 * MODULE/LEVEL:  gem/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    gem:     xpmnu, xpmxu, ypmnu, ypmxu, ximnz, ximxz, yimnz, yimxz
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gem:     xmpip1, xmpip2, ympip1, ympip2
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900307:  Original version from bottom of PLMAP.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900307
	 *===================================================================== */
	/* PROCEDURE: */
	/* - The mapping constants (slope and intercept) needed to convert from
	 *   input coordinates to plot coordinates  (i.e. normalized device
	 *   coordinates in the range 0. to 1.) are calculated here. */
	cmgem.xmpip1 = (cmgem.uplot.xmax - cmgem.uplot.xmin)/(cmgem.zdata.xmax - cmgem.zdata.xmin);
	cmgem.xmpip2 = -cmgem.xmpip1*cmgem.zdata.xmax + cmgem.uplot.xmax;
	cmgem.ympip1 = (cmgem.uplot.ymax - cmgem.uplot.ymin)/(cmgem.zdata.ymax - cmgem.zdata.ymin);
	cmgem.ympip2 = -cmgem.ympip1*cmgem.zdata.ymax + cmgem.uplot.ymax;

	return;

} /* end of function */

