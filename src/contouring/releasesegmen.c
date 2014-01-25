/** 
 * @file   releasesegmen.c
 * 
 * @brief  Relase storage for line segments
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "amf.h"
#include "contouring.h"

void 
releasesegments()
{
	int nerr;

	/*=====================================================================
	 * PURPOSE:  To release storage for contour line segments.
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *     mem:         sacmem, isacmem
	 *     contouring:  indexlevels, indexstarts, indexstops
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *     contouring:  indexlevels, indexstarts, indexstops
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *     sac:  relamb
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900315:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900315
	 *===================================================================== */
	/* PROCEDURE: */
	nerr = 0;

	/* - Release space for contour level values. */

	relamb( cmmem.sacmem, cmcontouring.indexlevels, &nerr );
	cmcontouring.indexlevels = 0;

	/* - Release space for segment start point numbers. */

	relamb( cmmem.sacmem, cmcontouring.indexstarts, &nerr );
	cmcontouring.indexstarts = 0;

	/* - Release space for segment stop point numbers. */

	relamb( cmmem.sacmem, cmcontouring.indexstops, &nerr );
	cmcontouring.indexstops = 0;

	return;
}

