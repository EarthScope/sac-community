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
#include "contouring.h"
#include "debug.h"

extern struct contour contour;
CONTOURING_EXTERN

void
releasesegments() {

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
	 * MODIFICATION HISTORY:
	 *    900315:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900315
	 *===================================================================== */
    /* PROCEDURE: */

    /* - Release space for contour level values. */

    FREE(contour.segments);
    cmcontouring.indexlevels = 0;
    cmcontouring.indexstarts = 0;
    cmcontouring.indexstops = 0;

    return;
}
