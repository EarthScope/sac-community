/** 
 * @file   getcontseg.c
 * 
 * @brief  Get info about a segment
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "amf.h"
#include "contouring.h"

void 
getcontseg(number, level, start, stop)
int number, *level, *start, *stop;
{
        int *Isacmem;


	/*=====================================================================
	 * PURPOSE:  To get information about an existing contouring segment.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    number:  The previous segment number. Set to 0 to initialize.[i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    level:   The contour level index number [i]
	 *    start:   The index to the starting point of the segment. [i]
	 *    stop:    The index to the stopping point of the segment. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mem:  sacmem, isacmem
	 *    contouring:  numsegments, indexlevels, indexstarts, indexstops
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900315:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900315
	 *===================================================================== */
	/* PROCEDURE: */
	if( number <= cmcontouring.numsegments ){
                Isacmem = (int*)cmmem.sacmem[cmcontouring.indexlevels];
		*level = *(Isacmem + number - 1);
                Isacmem = (int*)cmmem.sacmem[cmcontouring.indexstarts];
		*start = *(Isacmem + number - 1);
                Isacmem = (int*)cmmem.sacmem[cmcontouring.indexstops];
		*stop = *(Isacmem + number - 1);
	}
	else{
		fprintf( stdout, "Illegal segment number: %d \n", number );
		exit(0);
	}

	return;
}

