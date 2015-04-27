/** 
 * @file   newcontlabel.c
 * 
 * @brief  Put info about a lable
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"
extern struct contour contour;

void
newcontlabel(jpoint, jtype, angle, jtext, number)
     int jpoint, jtype;
     double angle;
     int jtext, *number;
{

        /*=====================================================================
	 * PURPOSE:  To put information about a new contouring line label.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    jpoint:  The index to the point where label is to be placed. [i]
	 *    jtype:   The type (status) of the label. [i]
	 *    angle:   The angle at which the label is to be written. [r]
	 *    jtext:   The index to the list of text for labels. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    number:  The label number. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    contouring:   maxlabels
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    mem:          sacmem, isacmem
	 *    contouring:   numlabels, indexlabelpoint, indexlabeltype,
	 *                  indexlabelangle, indexlabeltext
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900418:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900418
	 *===================================================================== */
    /* PROCEDURE: */
    if (cmcontouring.numlabels < cmcontouring.maxlabels) {
        cmcontouring.numlabels = cmcontouring.numlabels + 1;
        *number = cmcontouring.numlabels;
        contour.label[*number - 1].point = jpoint;
        contour.label[*number - 1].type = jtype;
        contour.label[*number - 1].angle = angle;
        contour.label[*number - 1].text = jtext;
    } else {
        fprintf(stdout, "No more room for label storage.\n");
        exit(0);
    }
    return;
}
