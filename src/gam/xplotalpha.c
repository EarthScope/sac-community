#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bool.h"
#include "gam.h"


#include "dfm.h"

void  xplotalpha(int *nerr)
{

	/*=====================================================================
	 * PURPOSE:  To execute the action command PLOTALPHA.
	 *           This command calls readalpha with a flag that tells it to 
	 *           plot also.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  GAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GEM:     LXGEN, XDELTA, XFIRST, LYLIM, YIMN, YIMX
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  XRA
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920713:  Original version.
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	xrtab( TRUE, nerr );
	return;

} /* end of function */

