/** 
 * @file   releaselabels.c
 * 
 * @brief  Relase storage for labels
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"
#include "debug.h"

extern struct contour contour;
CONTOURING_EXTERN

void
releaselabels() {

        /*=====================================================================
	 * purpose:  to release storage for contour label information.
	 *=====================================================================
	 * module/level:  contouring/5
	 *=====================================================================
	 * global input:
	 *     mach:
	 *     mem:         sacmem
	 *     contouring:  indexseglabelst, indexseglabelnu,
	 *                  indexseglabelfi, indexlabelpoint,
	 *                  indexlabeltype, indexlabelangle, indexlabeltext
	 *=====================================================================
	 * modification history:
	 *    900821:  shortened variable names to 15 characters max, to keep
	 *             things working under sunos 3.5:
	 *                indexseglabelstatus -> indexseglabelst
	 *                indexseglabelnumber -> indexseglabelnu
	 *                indexseglabelfirst  -> indexseglabelfi
	 *    900425:  original version.
	 *=====================================================================
	 * documented/reviewed:  900425
	 *===================================================================== */
    /* procedure: */

    /* - release space for each of the label attributes. */
    FREE(contour.labelseg);
    cmcontouring.indexseglabelst = 0;
    cmcontouring.indexseglabelnu = 0;
    cmcontouring.indexseglabelfi = 0;

    FREE(contour.label);
    cmcontouring.indexlabelpoint = 0;
    cmcontouring.indexlabeltype = 0;
    cmcontouring.indexlabelangle = 0;
    cmcontouring.indexlabeltext = 0;

    return;
}
