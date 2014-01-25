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
#include "amf.h"
#include "contouring.h"

void 
putcontlabel(number, jpoint, jtype, angle, jtext)
int number, jpoint, jtype;
double angle;
int jtext;
{
        int *Isacmem;


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
	if( number <= cmcontouring.numlabels ){
                Isacmem = (int *)cmmem.sacmem[cmcontouring.indexlabelpoint];
		*(Isacmem + number - 1) = jpoint;

                Isacmem = (int *)cmmem.sacmem[cmcontouring.indexlabeltype];
		*(Isacmem + number - 1) = jtype;

		*(cmmem.sacmem[cmcontouring.indexlabelangle] + number - 1) = angle;

                Isacmem = (int *)cmmem.sacmem[cmcontouring.indexlabeltext];
		*(Isacmem + number - 1) = jtext;
	}
	else{
		fprintf( stdout, "Illegal label number: %d \n", number );
		exit(0);
	}
	return;
}

