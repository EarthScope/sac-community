/** 
 * @file   allocpoints.c
 * 
 * @brief  Allocate storage for contour line points
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "contouring.h"
#include "debug.h"

extern struct contour contour;

void allocpoints(maxpoints, indexpoints, indexlinks, 
	 indexrlinks, indexaction, nerr)
     int maxpoints, *indexpoints, *indexlinks, *indexrlinks, *indexaction, 
	 *nerr;
{
  UNUSED(indexpoints);
  UNUSED(indexlinks);
  UNUSED(indexrlinks);
  UNUSED(indexaction);


	/*=====================================================================
	 * PURPOSE:  To allocate storage for contour line points.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    maxpoints:   Maximum number of points to allocate storage for.[i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    indexpoints: Index to storage space for points. [i]
	 *    indexlinks:  Index to storage space for (forward) links. [i]
	 *    indexrlinks: Index to storage space for reverse (backward) links. [i]
	 *    indexaction: Index to storage space for action attribute. [i]
	 *    nerr:        Set to 0 if no error occurred. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *     mem:  sacmem, isacmem
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *     sac:  allamb
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900412:  Added storage for reverse links.
	 *    900405:  Added storage for action attribute.
	 *    900315:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900412
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

  contour.points = (struct points *) malloc(sizeof(struct points) * maxpoints);
}
