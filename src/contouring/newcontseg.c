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
#include "amf.h"
#include "contouring.h"

void 
newcontseg(level, start, stop, number)
int level, start, stop, *number;
{
        int *Isacmem;


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
	if( cmcontouring.numsegments < cmcontouring.maxsegments ){
		cmcontouring.numsegments = cmcontouring.numsegments + 1;
		*number = cmcontouring.numsegments;

                Isacmem = (int*)cmmem.sacmem[cmcontouring.indexlevels];
		*(Isacmem + *number - 1) = level;

                Isacmem = (int*)cmmem.sacmem[cmcontouring.indexstarts];
		*(Isacmem + *number - 1) = start;

                Isacmem = (int*)cmmem.sacmem[cmcontouring.indexstops];
		*(Isacmem + *number - 1) = stop;
	}
	else{
		fprintf( stdout, "No more room for segment storage\n" );
		exit(0);
	}
	return;
}

