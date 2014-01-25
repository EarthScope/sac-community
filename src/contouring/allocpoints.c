/** 
 * @file   allocpoints.c
 * 
 * @brief  Allocate storage for contour line points
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "amf.h"

void allocpoints(maxpoints, indexpoints, indexlinks, 
	 indexrlinks, indexaction, nerr)
     int maxpoints, *indexpoints, *indexlinks, *indexrlinks, *indexaction, 
	 *nerr;
{
	int nrerr;



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
	 *     sac:  allamb, relamb
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

	/* - Allocate space for points. Two values are needed per point. */

	allamb( &cmmem, 2*maxpoints, indexpoints, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Allocate space for links. */

	allamb( &cmmem, maxpoints, indexlinks, nerr );
	if( *nerr != 0 ){
		relamb( cmmem.sacmem, *indexpoints, &nrerr );
		*indexpoints = 0;
		*indexlinks = 0;
		*indexrlinks = 0;
		*indexaction = 0;
		goto L_8888;
		}

	/* - Allocate space for reverse links. */

	allamb( &cmmem, maxpoints, indexrlinks, nerr );
	if( *nerr != 0 ){
		relamb( cmmem.sacmem, *indexpoints, &nrerr );
		relamb( cmmem.sacmem, *indexlinks, &nrerr );
		*indexpoints = 0;
		*indexlinks = 0;
		*indexrlinks = 0;
		*indexaction = 0;
		goto L_8888;
		}

	/* - Allocate space for action attribute storage. */

	allamb( &cmmem, maxpoints, indexaction, nerr );
	if( *nerr != 0 ){
		relamb( cmmem.sacmem, *indexpoints, &nrerr );
		relamb( cmmem.sacmem, *indexlinks, &nrerr );
		relamb( cmmem.sacmem, *indexrlinks, &nrerr );
		*indexpoints = 0;
		*indexlinks = 0;
		*indexrlinks = 0;
		*indexaction = 0;
		goto L_8888;
		}

L_8888:
	return;

} /* end of function */

