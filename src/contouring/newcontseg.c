/** 
 * @file   newcontseg.c
 * 
 * @brief  Put info about a segment
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"

extern struct contour contour;
CONTOURING_EXTERN

void
newcontseg(int level, int start, int stop, int *number)
{

        /*=====================================================================
	 * PURPOSE:  To put information about a new contouring segment.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    level:   The contour level value index. [i]
	 *    start:   The index to the starting point of the segment. [i]
	 *    stop:    The index to the stopping point of the segment. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    number:  The segment number. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    contouring:  maxsegments
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    mem:         sacmem, isacmem
	 *    contouring:  numsegments, indexlevels, indexstarts, indexstops
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900315:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900315
	 *===================================================================== */
    /* PROCEDURE: */
    if (cmcontouring.numsegments < cmcontouring.maxsegments) {
        cmcontouring.numsegments = cmcontouring.numsegments + 1;
        *number = cmcontouring.numsegments;
        contour.segments[*number - 1].level = level;
        contour.segments[*number - 1].start = start;
        contour.segments[*number - 1].stop = stop;
    } else {
        fprintf(stdout, "No more room for segment storage\n");
        exit(0);
    }
    return;
}
