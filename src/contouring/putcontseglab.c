/** 
 * @file   putcontseglab.c
 * 
 * @brief  Put info about a label lin segment
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"

extern struct contour contour;
CONTOURING_EXTERN

void
putcontseglabel(int number, int status, int numlocs, int firstloc)
{

        /*=====================================================================
	 * PURPOSE:  To put (store) label information about an existing
	 *           contouring segment information.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    number:    The segment number. [i]
	 *    status:    The labeling status for this segment. [i]
	 *    numlocs:   The number of labels for this segment. [i]
	 *    firstloc:  The pointer to the first label for this segment. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    contouring:  numsegments, indexseglabelst, 
	 *                 indexseglabelnu, indexseglabelfi
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    mem:         sacmem, isacmem
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900821:  Shortened variable names to 15 characters max, to keep
	 *             things working under SunOS 3.5:
	 *                indexseglabelstatus -> indexseglabelst
	 *                indexseglabelnumber -> indexseglabelnu
	 *                indexseglabelfirst  -> indexseglabelfi
	 *    900418:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900418
	 *===================================================================== */
    /* PROCEDURE: */
    if (number <= cmcontouring.numsegments) {
        contour.labelseg[number - 1].status = status;
        contour.labelseg[number - 1].number = numlocs;
        contour.labelseg[number - 1].first = firstloc;
    } else {
        fprintf(stdout, "Illegal labeled segment number: %d \n", number);
    }
    return;
}
