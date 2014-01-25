/** 
 * @file   putcontseg.c
 * 
 * @brief  Put info about a line segment
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "amf.h"
#include "contouring.h"

void 
putcontseg(number, level, start, stop)
int number, level, start, stop;
{

        int *Isacmem;

	/*=====================================================================
	 * PURPOSE:  To put (store) information about an existing contouring
	 *           line segment.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    number:  The segment number. [i]
	 *    level:   The contour level index number. [i]
	 *    start:   The index to the starting point of the segment. [i]
	 *    stop:    The index to the stopping point of the segment. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    contouring:  numsegments, indexlevels, indexstarts, indexstops
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    mem:         sacmem, isacmem
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900315:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900315
	 *===================================================================== */
	/* PROCEDURE: */
	if( number <= cmcontouring.numsegments ){
                Isacmem = (int*)cmmem.sacmem[cmcontouring.indexlevels];
		*(Isacmem + number - 1) = level;

                Isacmem = (int*)cmmem.sacmem[cmcontouring.indexstarts];
		*(Isacmem + number - 1) = start;

                Isacmem = (int*)cmmem.sacmem[cmcontouring.indexstops];
		*(Isacmem + number - 1) = stop;
		}
	else{
		fprintf( stdout, "Illegal segment number: %d \n", number );
		}
	return;
}

