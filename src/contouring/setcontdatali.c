/** 
 * @file   setcontdatali.c
 * 
 * @brief  Set Contour Data Limits
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "contouring.h"

void 
setcontdatalim(ixstart, ixstop, iystart, iystop)
int ixstart, ixstop, iystart, iystop;
{
	/*=====================================================================
	 * PURPOSE:  To set the contour data limits to use in
	 *           subsequent contour plots.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    ixstart:  Starting index in x direction of data to plot. [i]
	 *              If <= 0, then start is beginning of x data.
	 *    ixstop:   Stopping index in x direction. [i]
	 *              If <= 0, then stop is end of x data.
	 *    iystart:  Starting index in y direction. [i]
	 *    iystop:   Stopping index in y direction. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/4
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    mach:     ixdatastart, ixdatastop, iydatastart, iydatastop
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900409:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900409
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Save input variables in common block. */
	cmcontouring.ixdatastart = ixstart;
	cmcontouring.ixdatastop = ixstop;
	cmcontouring.iydatastart = iystart;
	cmcontouring.iydatastop = iystop;

	return;
}

