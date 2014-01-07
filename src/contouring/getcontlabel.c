/** 
 * @file   getcontlabel.c
 * 
 * @brief  Get info about a line lable
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"

extern struct contour contour;

void 
getcontlabel(number, jpoint, jtype, angle, jtext)
int number, *jpoint, *jtype;
float *angle;
int *jtext;
{


	/*=====================================================================
	 * PURPOSE:  To get information about an existing contouring line label.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    number:  The label number. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
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
	 *    mem:          sacmem, isacmem
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900418:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900418
	 *===================================================================== */
	/* PROCEDURE: */
	if( number <= cmcontouring.numlabels ){
    *jpoint = contour.label[number-1].point;
    *jtype  = contour.label[number-1].type;
    *angle  = contour.label[number-1].angle;
    *jtext  = contour.label[number-1].text;

  }
	else{
		fprintf( stdout, "Illegal label number: %d \n", number );
		exit(0);
	}

	return;
}

