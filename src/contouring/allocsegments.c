/** 
 * @file   allocsegments.c
 * 
 * @brief  Allocate storage for contour line segments
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "amf.h"

void allocsegments(maxsegments, indexlevels, indexstarts, 
	 indexstops, nerr)
int maxsegments, *indexlevels, *indexstarts, *indexstops, *nerr;
{
	int nrerr;



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
	 * SUBROUTINES CALLED:
	 *     sac:  allamb, relamb
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900315:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900315
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Allocate space for contour level values. */

	allamb( &cmmem, maxsegments, indexlevels, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Allocate space for segment start point numbers. */

	allamb( &cmmem, maxsegments, indexstarts, nerr );
	if( *nerr != 0 ){
		relamb( cmmem.sacmem, *indexlevels, &nrerr );
		*indexlevels = 0;
		*indexstarts = 0;
		*indexstops = 0;
		goto L_8888;
		}

	/* - Allocate space for segment stop point numbers. */

	allamb( &cmmem, maxsegments, indexstops, nerr );
	if( *nerr != 0 ){
		relamb( cmmem.sacmem, *indexlevels, &nrerr );
		relamb( cmmem.sacmem, *indexstarts, &nrerr );
		*indexlevels = 0;
		*indexstarts = 0;
		*indexstops = 0;
		goto L_8888;
		}

L_8888:
	return;

} /* end of function */

