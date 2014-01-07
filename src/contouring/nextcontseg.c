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
#include "contouring.h"
#include "bool.h"
extern struct contour contour;
int
nextcontseg(number, level, start, stop)
int *number, *level, *start, *stop;
{
	int nextcontseg_v;

  struct segments *segs;

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
  segs = contour.segments;
 
L_1000:
	if( *number <= cmcontouring.numsegments ){
		*start = segs[*number-1].start;
		if( *start > 0 ){
			*level = segs[*number-1].level;
			*stop = segs[*number-1].stop;
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

