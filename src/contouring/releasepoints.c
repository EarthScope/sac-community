/** 
 * @file   releasepoints.c
 * 
 * @brief  Release points
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"
#include "debug.h"
extern struct contour contour;

void releasepoints()
{

	/*=====================================================================
	 * PURPOSE:  To release storage for contour line points.
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *     mem:         sacmem, isacmem
	 *     contouring:  indexpoints, indexlinks, indexaction
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *     contouring:  indexpoints, indexlinks, indexaction
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *     sac:  relamb
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900405:  Added release of action attribute storage block.
	 *    900315:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900405
	 *===================================================================== */
	/* PROCEDURE: */

	/* - Release space for points. */
  FREE(contour.points);
	cmcontouring.indexpoints = 0;
	cmcontouring.indexlinks = 0;
	cmcontouring.indexaction = 0;

	return;
}

