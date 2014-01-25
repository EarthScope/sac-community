/** 
 * @file   nextcontseg.c
 * 
 * @brief  Get info about the next segment
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "amf.h"
#include "contouring.h"
#include "bool.h"

int
nextcontseg(number, level, start, stop)
int *number, *level, *start, *stop;
{
	int nextcontseg_v;
        int *Isacmem1, *Isacmem2, *Isacmem3;


	/*=====================================================================
	 * PURPOSE:  To get information about the "next" contouring segment.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    number:  The previous segment number. Set to 0 to initialize.[i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    number:  The next segment number. [i]
	 *             DO NOT CHANGE THIS VALUE BETWEEN CALLS.
	 *    level:   The contour level index number. [i]
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
	/* - Initialize or increment segment pointer. */
	if( *number <= 0 ){
		*number = 1;
		}
	else{
		*number = *number + 1;
		}
        Isacmem1 = (int *)cmmem.sacmem[cmcontouring.indexstarts];
        Isacmem2 = (int *)cmmem.sacmem[cmcontouring.indexlevels];
        Isacmem3 = (int *)cmmem.sacmem[cmcontouring.indexstops];

L_1000:
	if( *number <= cmcontouring.numsegments ){
		*start = *(Isacmem1 + *number - 1);
		if( *start > 0 ){
			*level = *(Isacmem2 + *number - 1);
			*stop = *(Isacmem3 + *number - 1);
			}
		else{
			*number = *number + 1;
			goto L_1000;
			}
		nextcontseg_v = TRUE;
		}
	else{
		nextcontseg_v = FALSE;
		}

	return( nextcontseg_v );
}

