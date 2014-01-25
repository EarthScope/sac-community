/** 
 * @file   initcontattr.c
 * 
 * @brief  Initialize the contour module attributes
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mach.h"
#include "contouring.h"
#include "bool.h"


#define PI  M_PI

void
initcontattr()
{
	/*=====================================================================
	 * PURPOSE:  To initialize the contour module attributes.
	 *=====================================================================
	 * MODULE/LEVEL:  contouring/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:        PI
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    contouring:  kdatamode, klevelmode, klinemode, ktickmode,
	 *                 klabelmode, klabellist, nlabellist,
	 *                 nzlevellist, zlevelincrement, nznumlevels,
	 *                 nlinelist, linelist, nzregionlist, zregionlist,
	 *                 nticklist, iticklist, ticklength, tickspacing,
	 *                 lticksdown
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900430:  Added color attributes.
	 *    900405:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900405
	 *===================================================================== */
	/* PROCEDURE: */
	strcpy( kmcontouring.kdatamode, "ROWS    " );
	cmcontouring.ixdatastart = 0;
	cmcontouring.ixdatastop = 0;
	cmcontouring.iydatastart = 0;
	cmcontouring.iydatastop = 0;


        memset(kmcontouring.klistname,(int)' ',MCPFN);
        kmcontouring.klistname[MCPFN] = '\0';
        memcpy(kmcontouring.klistname,"OFF",3);

	strcpy( kmcontouring.klevelmode, "SCALE   " );
	cmcontouring.nzlevellist = 0;
	cmcontouring.zlevelincrement = -1.0;
	cmcontouring.nznumlevels = 20;

	strcpy( kmcontouring.klinemode, "ON      " );
	strcpy( kmcontouring.klinetype, "LIST    " );
	cmcontouring.nlinelist = 1;
	Linelist[1] = 1;
	cmcontouring.nzregionlist = 1;
	Zregionlist[1] = 0.0;

	strcpy( kmcontouring.kcolormode, "OFF     " );
	cmcontouring.ncolorlist = 7;
	strcpy( kmcontouring.kcolorlist[0], "RED     " );
	strcpy( kmcontouring.kcolorlist[1], "GREEN   " );
	strcpy( kmcontouring.kcolorlist[2], "BLUE    " );
	strcpy( kmcontouring.kcolorlist[3], "YELLOW  " );
	strcpy( kmcontouring.kcolorlist[4], "CYAN    " );
	strcpy( kmcontouring.kcolorlist[5], "MAGENTA " );
	strcpy( kmcontouring.kcolorlist[6], "BLACK   " );

	strcpy( kmcontouring.ktickmode, "OFF     " );
	cmcontouring.lticksdown = TRUE;
	cmcontouring.nticklist = 1;
	Iticklist[1] = -MACTIONTICK;
	cmcontouring.tickspacing = 0.1;
	cmcontouring.ticklength = 0.005;

	strcpy( kmcontouring.klabelmode, "OFF     " );
	cmcontouring.nlabellist = 1;
	strcpy( kmcontouring.klabellist[0], "ON              " );
	kmcontouring.kdecimal = '2';
	cmcontouring.minlabelspacing = 0.1;
	cmcontouring.deslabelspacing = 0.2;
	cmcontouring.maxlabelspacing = 0.3;
	cmcontouring.desiredangle = 0.25*PI;
	cmcontouring.widthlabels = 0.0075;
	cmcontouring.heightlabels = 0.0075;


	return;
} 

