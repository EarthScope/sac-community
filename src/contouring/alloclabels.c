/** 
 * @file   alloclabels.c
 *
 * @brief  Allocate storage for contour labels
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "contouring.h"

struct contour contour;


void alloclabels(maxsegments, maxlabels, indexseglabelst, 
	 indexseglabelnu, indexseglabelfi, indexlabelpoint, indexlabeltype, 
	 indexlabelangle, indexlabeltext, nerr)
int maxsegments, maxlabels, *indexseglabelst, *indexseglabelnu, 
	 *indexseglabelfi, *indexlabelpoint, *indexlabeltype, *indexlabelangle, 
	 *indexlabeltext, *nerr;
{

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

  contour.label = (struct label *) malloc(sizeof(struct label) * maxlabels);
  contour.labelseg = (struct labelseg *) malloc(sizeof(struct labelseg) * maxsegments);
}
