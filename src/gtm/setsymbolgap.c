
#include "gtm.h"
#include "co.h"


void /*FUNCTION*/ setsymbolgap(gap)
double gap;
{



	/*=====================================================================
	 * PURPOSE: To set the symbol gap attribute.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    gap:     The new symbol gap attribute. [f]
	 *             This is the minimum viewport gap (space) between
	 *             adjacent plotted symbols in the same line segment.
	 *             A value of 0. means that a symbol will be plotted
	 *             at every data point in line segment.
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gtm:     symgap
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861027:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861027
	 *===================================================================== */
	/* PROCEDURE: */
	cmgtm.symgap = fmin( 1.0, fmax( 0.0, gap ) );

       
	return;

} /* end of function */

