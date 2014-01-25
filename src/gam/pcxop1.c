#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gam.h"
#include "gem.h"
#include "co.h"
#include "bool.h"


#include "gtm.h"
#include "gdm.h"

#define PI M_PI

void pcxop1(int iop1)
{
	int ilines, j;
	float arrowd, arrowl, dtheta, hgap, hloc, hloc1, hloc2, hloc3, 
	 hloc4, radius, slope, theta, vgap, vloc, vloc1, vloc2, vloc3, 
	 vloc4, wtick, x, xa1, xa2, xlen, y, ya1, ya2, ylen;


	/*=====================================================================
	 * PURPOSE:
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *=====================================================================
	 * MODULE/LEVEL:
	 *=====================================================================
	 * GLOBAL INPUT:
	 *  gem: icline
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *=====================================================================
	 * GLOBAL COUPLING:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    SLOPE:   Slope of line segment used to draw arrow. [f]
	 *             Saved as local variable to handle request for a zero
	 *             length arrow to be drawn with previous slope and origin.
	 *=====================================================================
	 * ASSUMPTIONS:
	 *=====================================================================
	 * LIMITATIONS:
	 *=====================================================================
	 * KNOWN ERRORS:
	 *=====================================================================
	 * EXTERNAL DEPENDENCIES:
	 * MODIFICATION HISTORY:
	 *  910301: Changed iline to icline. (wct)
	 *  ??????: Original version
	 *===================================================================== */
	/* PROCEDURE: */
	switch( iop1 ){
		case 1: goto L_5100;
		case 2: goto L_5200;
		case 3: goto L_5300;
		case 4: goto L_5400;
		case 5: goto L_5500;
		case 6: goto L_5600;
		case 7: goto L_5700;
		case 8: goto L_5800;
		}

	/* -- G:  Reset origin and make it "global."
	 *        Origin will not be reset until next O command. */
L_5100:
	cmgam.lglori = TRUE;
	cmgam.xori = cmgam.xcdp;
	cmgam.yori = cmgam.ycdp;
	goto L_8888;

	/* -- O:  Reset origin by turning off "global origin" flag. */
L_5200:
	cmgam.lglori = FALSE;
	goto L_8888;

	/* -- L:  Draw line from origin to cdp. */
L_5300:
	worldline( cmgam.xori, cmgam.yori, cmgam.xcdp, cmgam.ycdp );
	goto L_8888;

	/* -- A:  Draw an arrow from ORI to CDP. */
L_5400:
	if( cmgam.lpcavi ){
		worldline( cmgam.xori, cmgam.yori, cmgam.xcdp, cmgam.ycdp );
		}
	else{
		worldmove( cmgam.xcdp, cmgam.ycdp );
		}
	ylen = cmgam.ycdp - cmgam.yori;
	xlen = cmgam.xcdp - cmgam.xori;
	slope = atan2( ylen, xlen );
	arrowl = cmgam.pcalen*cmgam.pcamul;
	xa1 = cmgam.xcdp - arrowl*cos( slope + cmgam.pcaang );
	ya1 = cmgam.ycdp - arrowl*sin( slope + cmgam.pcaang );
	xa2 = cmgam.xcdp - arrowl*cos( slope - cmgam.pcaang );
	ya2 = cmgam.ycdp - arrowl*sin( slope - cmgam.pcaang );
	worlddraw( xa1, ya1 );
	worlddraw( xa2, ya2 );
	worlddraw( cmgam.xcdp, cmgam.ycdp );
	if( cmgam.lpcafi ){
		ilines = cmgem.icline;
		setlinestyle( 1 );
		arrowd = 0.001;
L_5420:
		arrowl = arrowl - arrowd;
		if( arrowl > 0 ){
			xa1 = cmgam.xcdp - arrowl*cos( slope + cmgam.pcaang );
			ya1 = cmgam.ycdp - arrowl*sin( slope + cmgam.pcaang );
			xa2 = cmgam.xcdp - arrowl*cos( slope - cmgam.pcaang );
			ya2 = cmgam.ycdp - arrowl*sin( slope - cmgam.pcaang );
			worldmove( xa1, ya1 );
			worlddraw( xa2, ya2 );
			goto L_5420;
			}
		worldmove( cmgam.xcdp, cmgam.ycdp );
		cmgem.icline = ilines;
		setlinestyle( cmgem.icline );
		}
	goto L_8888;

	/* -- R:  Draw a rectangle with opposing corners at origin and cdp. */
L_5500:
	worldmove( Xrect[4], Yrect[4] );
	for( j = 1; j <= 4; j++ ){
		worlddraw( Xrect[j], Yrect[j] );
		}
	goto L_8888;

	/* -- C:  Draw a circle centered at ORI with radius of |ORI-CDP|. */
L_5600:
	radius = sqrt( powi(cmgam.xori - cmgam.xcdp,2) + powi(cmgam.yori - 
	 cmgam.ycdp,2) );
	worldsector( cmgam.xori, cmgam.yori, radius, 0., 360., cmgam.pcdegi );
	goto L_8888;

	/* -- N:  Draw n-sided polygon of "radius" |CDP-ORI|. */
L_5700:
	radius = sqrt( powi(cmgam.xcdp - cmgam.xori,2) + powi(cmgam.ycdp - 
	 cmgam.yori,2) );
	theta = atan2( cmgam.ycdp - cmgam.yori, cmgam.xcdp - cmgam.xori );
	dtheta = 2.*PI/(float)( cmgam.npcpsi );
	worldmove( cmgam.xcdp, cmgam.ycdp );
	for( j = 1; j <= cmgam.npcpsi; j++ ){
		theta = theta + dtheta;
		x = cmgam.xori + radius*cos( theta );
		y = cmgam.yori + radius*sin( theta );
		worlddraw( x, y );
		}
	goto L_8888;

	/* --- B:  Draw horizontal and vertical border tick marks. */
L_5800:
	wtick = 0.02;
	if( cmgam.lbdrrq && cmgam.nhtick > 0 ){
		hgap = (cmgem.view.xmax - cmgem.view.xmin - 2.*VSMALL)/(cmgam.nhtick + 
		 1);
		hloc = hgap;
		vloc1 = cmgem.view.ymin + VSMALL;
		vloc2 = cmgem.view.ymin + VSMALL + wtick;
		vloc3 = cmgem.view.ymax - VSMALL;
		vloc4 = cmgem.view.ymax - VSMALL - wtick;
		for( j = 1; j <= cmgam.nhtick; j++ ){
			line( hloc, vloc1, hloc, vloc2 );
			line( hloc, vloc3, hloc, vloc4 );
			hloc = hloc + hgap;
			}
		}
	if( cmgam.lbdrrq && cmgam.nvtick > 0 ){
		vgap = (cmgem.view.ymax - cmgem.view.ymin - 2.*VSMALL)/(cmgam.nvtick + 
		 1);
		vloc = vgap;
		hloc1 = cmgem.view.xmin + VSMALL;
		hloc2 = cmgem.view.xmin + VSMALL + wtick;
		hloc3 = cmgem.view.xmax - VSMALL;
		hloc4 = cmgem.view.xmax - VSMALL - wtick;
		for( j = 1; j <= cmgam.nvtick; j++ ){
			line( hloc1, vloc, hloc2, vloc );
			line( hloc3, vloc, hloc4, vloc );
			vloc = vloc + vgap;
			}
		}
	goto L_8888;

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    860106:  Modified rectangle drawing logic to correctly
	 *             handle rotation of rectangle inside a macro.
	 *    830623:  Added enviroment options with arbitrary integer argument.
	 *    810000:  Original version.
	 *===================================================================== */

} /* end of function */

