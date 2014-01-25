/** 
 * @file   alloclabels.c
 *
 * @brief  Allocate storage for contour labels
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "amf.h"

void alloclabels(maxsegments, maxlabels, indexseglabelst, 
	 indexseglabelnu, indexseglabelfi, indexlabelpoint, indexlabeltype, 
	 indexlabelangle, indexlabeltext, nerr)
int maxsegments, maxlabels, *indexseglabelst, *indexseglabelnu, 
	 *indexseglabelfi, *indexlabelpoint, *indexlabeltype, *indexlabelangle, 
	 *indexlabeltext, *nerr;
{
	int nrerr;



	/*=====================================================================
	 * PURPOSE:  To allocate storage for contour label information.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    maxsegments:  Maximum number of segments to allocate storage for.[i]
	 *    maxlabels:    Maximum number of labels to allocate storage for.[i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    indexseglabelst:  Index to storage for segment label status. [i]
	 *    indexseglabelnu:  Index to storage for number of segment label
	 *                          positions. [i]
	 *    indexseglabelfi:   Index to storage for first segment label 
	 *                          position. [i]
	 *    indexlabelpoint:      Index to storage for label pointer. [i]
	 *    indexlabeltype:       Index to storage for label type. [i]
	 *    indexlabelangle:      Index to storage for label angle. [i]
	 *    indexlabeltext:       Index to storage for label text pointer. [i]
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
	 *    900821:  Shortened variable names to 15 characters max, to keep
	 *             things working under SunOS 3.5:
	 *               indexseglabelstatus -> indexseglabelst
	 *               indexseglabelnumber -> indexseglabelnu
	 *               indexseglabelfirst  -> indexseglabelfi
	 *    900419:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900419
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Assume the worst. */

	*indexseglabelst = 0;
	*indexseglabelnu = 0;
	*indexseglabelfi = 0;
	*indexlabelpoint = 0;
	*indexlabeltype = 0;
	*indexlabelangle = 0;
	*indexlabeltext = 0;

	/* - Allocate space for segment label status information. */

	allamb( &cmmem, maxsegments, indexseglabelst, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Allocate space for segment label number locs information. */

	allamb( &cmmem, maxsegments, indexseglabelnu, nerr );
	if( *nerr != 0 ){
		relamb( cmmem.sacmem, *indexseglabelst, &nrerr );
		goto L_8888;
		}

	/* - Allocate space for segment label number locs information. */

	allamb( &cmmem, maxsegments, indexseglabelfi, nerr );
	if( *nerr != 0 ){
		relamb( cmmem.sacmem, *indexseglabelst, &nrerr );
		relamb( cmmem.sacmem, *indexseglabelnu, &nrerr );
		goto L_8888;
		}

	/* - Allocate space for label pointer. */

	allamb( &cmmem, maxlabels, indexlabelpoint, nerr );
	if( *nerr != 0 ){
		relamb( cmmem.sacmem, *indexseglabelst, &nrerr );
		relamb( cmmem.sacmem, *indexseglabelnu, &nrerr );
		relamb( cmmem.sacmem, *indexseglabelfi, &nrerr );
		goto L_8888;
		}

	/* - Allocate space for label type. */

	allamb( &cmmem, maxlabels, indexlabeltype, nerr );
	if( *nerr != 0 ){
		relamb( cmmem.sacmem, *indexseglabelst, &nrerr );
		relamb( cmmem.sacmem, *indexseglabelnu, &nrerr );
		relamb( cmmem.sacmem, *indexseglabelfi, &nrerr );
		relamb( cmmem.sacmem, *indexlabelpoint, &nrerr );
		goto L_8888;
		}

	/* - Allocate space for label angle. */

	allamb( &cmmem, maxlabels, indexlabelangle, nerr );
	if( *nerr != 0 ){
		relamb( cmmem.sacmem, *indexseglabelst, &nrerr );
		relamb( cmmem.sacmem, *indexseglabelnu, &nrerr );
		relamb( cmmem.sacmem, *indexseglabelfi, &nrerr );
		relamb( cmmem.sacmem, *indexlabelpoint, &nrerr );
		relamb( cmmem.sacmem, *indexlabeltype, &nrerr );
		goto L_8888;
		}

	/* - Allocate space for label text pointer. */

	allamb( &cmmem, maxlabels, indexlabeltext, nerr );
	if( *nerr != 0 ){
		relamb( cmmem.sacmem, *indexseglabelst, &nrerr );
		relamb( cmmem.sacmem, *indexseglabelnu, &nrerr );
		relamb( cmmem.sacmem, *indexseglabelfi, &nrerr );
		relamb( cmmem.sacmem, *indexlabelpoint, &nrerr );
		relamb( cmmem.sacmem, *indexlabeltype, &nrerr );
		relamb( cmmem.sacmem, *indexlabelangle, &nrerr );
		goto L_8888;
		}

L_8888:
	return;

} /* end of function */

