/** 
 * @file   putcontrlink.c
 * 
 * @brief  Put the reverse link for a line point
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "amf.h"
#include "contouring.h"

void 
putcontrlink(number, rlink)
int number, rlink;
{
        int *Isacmem;


	/*=====================================================================
	 * PURPOSE:  To put (store) the reverse link for an existing 
	 *           contouring line point.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    number:  The point number. [i]
	 *    rlink:   The reverse link to the previous point in segment. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    contouring:   numpoints, indexrlinks
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    mem:          sacmem, isacmem
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900412:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900412
	 *===================================================================== */
	/* PROCEDURE: */
	if( number <= cmcontouring.numpoints ){
                Isacmem = (int*)cmmem.sacmem[cmcontouring.indexrlinks];
		*(Isacmem + number - 1) = rlink;
	}
	else{
		fprintf( stdout, "Illegal point number:%d putcontrlink\n", number );
		exit(0);
	}
	return;
}

