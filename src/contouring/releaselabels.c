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
#include "amf.h"
#include "contouring.h"

void 
releaselabels()
{
	int nerr, nrerr;

	/*=====================================================================
	 * PURPOSE:  To release storage for contour label information.
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *     mach:
	 *     mem:         sacmem
	 *     contouring:  indexseglabelst, indexseglabelnu,
	 *                  indexseglabelfi, indexlabelpoint,
	 *                  indexlabeltype, indexlabelangle, indexlabeltext
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *     sac:  relamb
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900821:  Shortened variable names to 15 characters max, to keep
	 *             things working under SunOS 3.5:
	 *                indexseglabelstatus -> indexseglabelst
	 *                indexseglabelnumber -> indexseglabelnu
	 *                indexseglabelfirst  -> indexseglabelfi
	 *    900425:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900425
	 *===================================================================== */
	/* PROCEDURE: */
	nerr = 0;

	/* - Release space for each of the label attributes. */

	relamb( cmmem.sacmem, cmcontouring.indexseglabelst, &nrerr );
	cmcontouring.indexseglabelst = 0;

	relamb( cmmem.sacmem, cmcontouring.indexseglabelnu, &nerr );
	cmcontouring.indexseglabelnu = 0;

	relamb( cmmem.sacmem, cmcontouring.indexseglabelfi, &nerr );
	cmcontouring.indexseglabelfi = 0;

	relamb( cmmem.sacmem, cmcontouring.indexlabelpoint, &nerr );
	cmcontouring.indexlabelpoint = 0;

	relamb( cmmem.sacmem, cmcontouring.indexlabeltype, &nerr );
	cmcontouring.indexlabeltype = 0;

	relamb( cmmem.sacmem, cmcontouring.indexlabelangle, &nerr );
	cmcontouring.indexlabelangle = 0;

	relamb( cmmem.sacmem, cmcontouring.indexlabeltext, &nerr );
	cmcontouring.indexlabeltext = 0;

	return;
}


