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
#include "amf.h"
#include "contouring.h"

void
putcontseglabel(number, status, numlocs, firstloc)
int number, status, numlocs, firstloc;
{

        int *Isacmem;

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
	if( number <= cmcontouring.numsegments ){
                Isacmem = (int *)cmmem.sacmem[cmcontouring.indexseglabelst];
		*(Isacmem + number - 1) = status;

                Isacmem = (int *)cmmem.sacmem[cmcontouring.indexseglabelnu];
		*(Isacmem + number - 1) = numlocs;

                Isacmem = (int *)cmmem.sacmem[cmcontouring.indexseglabelfi];
		*(Isacmem + number - 1) = firstloc;
		}
	else{
		fprintf( stdout, "Illegal labeled segment number: %d \n", 
		 number );
		}
	return;
}

