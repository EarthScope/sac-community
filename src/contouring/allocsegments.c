/** 
 * @file   allocsegments.c
 * 
 * @brief  Allocate storage for contour line segments
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "contouring.h"
#include "debug.h"

extern struct contour contour;

void allocsegments(maxsegments, indexlevels, indexstarts, 
	 indexstops, nerr)
int maxsegments, *indexlevels, *indexstarts, *indexstops, *nerr;
{
  UNUSED(indexlevels);
  UNUSED(indexstarts);
  UNUSED(indexstops);
  UNUSED(nerr);

	/*=====================================================================
	 * PURPOSE:  To allocate storage for contour line segments.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    maxsegments:  Maximum number of segments to allocate storage for.[i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    indexlevels:  Index to storage space for z levels. [i]
	 *    indexstarts:  Index to storage space for start points. [i]
	 *    indexstops:   Index to storage space for stop points. [i]
	 *    nerr:  Set to 0 if no error occurred. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *     mem:  sacmem, isacmem
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900315:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900315
	 *===================================================================== */
	/* PROCEDURE: */

  contour.segments = (struct segments *) malloc(sizeof(struct segments) * maxsegments);



} /* end of function */

