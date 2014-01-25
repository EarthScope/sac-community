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
#include "amf.h"
#include "contouring.h"

void 
newcontlabel(jpoint, jtype, angle, jtext, number)
int jpoint, jtype;
double angle;
int jtext, *number;
{
        int *Isacmem;


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
	if( cmcontouring.numlabels < cmcontouring.maxlabels ){
		cmcontouring.numlabels = cmcontouring.numlabels + 1;
		*number = cmcontouring.numlabels;
                Isacmem = (int*)cmmem.sacmem[cmcontouring.indexlabelpoint];
		*(Isacmem + *number - 1) = jpoint;

                Isacmem = (int*)cmmem.sacmem[cmcontouring.indexlabeltype];
		*(Isacmem + *number - 1) = jtype;

		*(cmmem.sacmem[cmcontouring.indexlabelangle] + *number - 1) = angle;

                Isacmem = (int*)cmmem.sacmem[cmcontouring.indexlabeltext];
		*(Isacmem + *number - 1) = jtext;
	}
	else{
		fprintf( stdout, "No more room for label storage.\n" );
		exit(0);
	}
	return;
}

