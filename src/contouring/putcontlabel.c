/** 
 * @file   putcontlabel.c
 * 
 * @brief  Put info about a label
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
putcontlabel(number, jpoint, jtype, angle, jtext)
     int number, jpoint, jtype;
     double angle;
     int jtext;
{

        /*=====================================================================
	 * PURPOSE:  To put information about an existing contouring line label.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    number:  The label number. [i]
	 *    jpoint:  The index to the point where label is to be placed. [i]
	 *    jtype:   The type (status) of the label. [i]
	 *    angle:   The angle at which the label is to be written. [r]
	 *    jtext:   The index to the list of text for labels. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    contouring:   numlabels, indexlabelpoint, indexlabeltype,
	 *                  indexlabelangle, indexlabeltext
	 *=====================================================================
	 *    mem:          sacmem, isacmem
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900418:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900418
	 *===================================================================== */
    /* PROCEDURE: */
    if (number <= cmcontouring.numlabels) {
        contour.label[number - 1].point = jpoint;
        contour.label[number - 1].type = jtype;
        contour.label[number - 1].angle = angle;
        contour.label[number - 1].text = jtext;

    } else {
        fprintf(stdout, "Illegal label number: %d \n", number);
        exit(0);
    }
    return;
}
