/** 
 * @file   calccontlabs.c
 * 
 * @brief  Calculate Contour line label locations
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"

#define	MPOINTS	3

void 
calccontlabels()
{
	int nerr;



	/*=====================================================================
	 * PURPOSE: To calculate contouring line label locations.
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    contouring:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:   AllocSegments, CalcContLabel1, CalcContLabel2, 
	 *           CalcContLabel3, CalcContLabel4
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900821:  Shortened parameter names to 15 characters to keep things
	 *             working under SunOS 3.5:
	 *                  indexseglabelstatus -> indexseglabelst
	 *                  indexseglabelnumber -> indexseglabelnu
	 *                  indexseglabelfirst  -> indexseglabelfi
	 *    900419:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900419
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Allocate additional space in which to store segment label information. */
	cmcontouring.maxlabels = 4*cmcontouring.numsegments;
	alloclabels( cmcontouring.maxsegments, cmcontouring.maxlabels, 
	 &cmcontouring.indexseglabelst, &cmcontouring.indexseglabelnu, 
	 &cmcontouring.indexseglabelfi, &cmcontouring.indexlabelpoint, 
	 &cmcontouring.indexlabeltype, &cmcontouring.indexlabelangle, 
	 &cmcontouring.indexlabeltext, &nerr );
	cmcontouring.numlabels = 0;

	/* - First pass to determine initial candidate locations. */

	calccontlabel1();

	/* - Second pass to iterate to select locations from candidate locations. */

	/*      call CalcContLabel2 */

	/* - Third pass to either select or reject remaining locations. */

	/*      call CalcContLabel3 */

	/* - Fourth pass to modify segment storage to accomidate selected labels. */

	calccontlabel4();

	return;
}

