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
#include "amf.h"
#include "contouring.h"

void releasepoints()
{
	int nerr;

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
	nerr = 0;

	/* - Release space for points. */

	relamb( cmmem.sacmem, cmcontouring.indexpoints, &nerr );
	cmcontouring.indexpoints = 0;

	/* - Release space for links. */

	relamb( cmmem.sacmem, cmcontouring.indexlinks, &nerr );
	cmcontouring.indexlinks = 0;

	/* - Release space for action attributes. */

	relamb( cmmem.sacmem, cmcontouring.indexaction, &nerr );
	cmcontouring.indexaction = 0;

	return;
}

