/** 
 * @file   getcontrlink.c
 * 
 * @brief  Get a reverse link for a point
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"
extern struct contour contour;
void
getcontrlink(number, rlink)
     int number, *rlink;
{

        /*=====================================================================
	 * PURPOSE:  To get the reverse link for an existing 
	 *           contouring line point.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    number:  The point number. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    rlink:   The reverse link to the previous point in segment. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    contouring:   numpoints, indexrlinks
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900412:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900412
	 *===================================================================== */
    /* PROCEDURE: */
    if (number <= cmcontouring.numpoints) {
        *rlink = contour.points[number - 1].rlink;
    } else {
        fprintf(stdout, "Illegal point number: %d getcontrlink\n", number);
        exit(0);
    }

    return;
}
