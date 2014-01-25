/** 
 * @file   clipdp.c
 * 
 * @brief  Clip a line segment to a rectangle
 * 
 */

#include "bot.h"

#define	IABOVE	8
#define	IBELOW	4
#define	ILEFT	1
#define	IRIGHT	2

/** 
 * Clip a line segment to a rectangle
 * 
 * @param xdp 
 *    Array of unclipped x data points, length = 2
 *    On Output, array of clipped x data points
 * @param ydp 
 *    Array of unclipped y data points, length = 2
 *    On Output, array of clipped y data points
 * @param ildp 
 *    Array of unclipped data point location values, length = 2
 *    See subroutine @ref locdp() for complete definition.
 * @param xrect 
 *    Array of x rectangle values, length = 2
 *    Left Edge First
 * @param yrect 
 *    Array of y rectangle values, length = 2
 *    Bottom Edge First
 * @param ncdp 
 *    Length of clipped data arrays
 *    - 0 if line segment was outside of the rectangle
 *    - 2 if all or part of the line segment was inside the rectangle
 *
 * @note Global Coupling: 
 * - Values for parameters IABOVE, IBELOW, ILEFT, and IRIGHT must be
 *   the same in the clipping subroutines: CLIP, BLANK, and LOCDP.
 *   These parameters are defined in source of subroutine LOCDP.
 * 
 * @note Local Variables
 *  - J1,J2:   Indices into XDP, YDP, and ILDP arrays.
 *             Indices are swapped instead of array values in algorithm.

 * @note Assumptions:
 *  -  XRECT(2) > XRECT(1) and YRECT(2) > YRECT(1).
 *
 * @date   840105:  Corrected bug involving line segment outside rectangle.
 * @date   811222:  Original version.
 *
 */
void 
clipdp(float *xdp, 
       float *ydp, 
       int   *ildp, 
       float *xrect, 
       float *yrect, 
       int   *ncdp)
{
	int j1, j2;
	float dxdp, dydp;

	int   *const Ildp = &ildp[0] - 1;
	float *const Xdp = &xdp[0] - 1;
	float *const Xrect = &xrect[0] - 1;
	float *const Ydp = &ydp[0] - 1;
	float *const Yrect = &yrect[0] - 1;


	/* - Each pass through loop moves one end closer to the rectangle.
	 *   Complete clipping may require several iterations. 
	 */
L_2000:
	;

	/* -- Set up indices so that the "J1" point is outside rectangle. */
	if( Ildp[1] != 0 ){
		j1 = 1;
		j2 = 2;
	}
	else{
		j1 = 2;
		j2 = 1;
	}

	/* -- Compute length of line segment in x and y directions. */
	dxdp = Xdp[j2] - Xdp[j1];
	dydp = Ydp[j2] - Ydp[j1];

	/* -- Move "J1" point toward rectangle. */
	if( ( Ildp[j1] & IBELOW ) != 0 ){
		Xdp[j1] = Xdp[j1] + (Yrect[1] - Ydp[j1])*dxdp/dydp;
		Ydp[j1] = Yrect[1];
	}
	else if( ( Ildp[j1] & IABOVE ) != 0 ){
		Xdp[j1] = Xdp[j1] + (Yrect[2] - Ydp[j1])*dxdp/dydp;
		Ydp[j1] = Yrect[2];
	}
	else if( ( Ildp[j1] & ILEFT ) != 0 ){
		Ydp[j1] = Ydp[j1] + (Xrect[1] - Xdp[j1])*dydp/dxdp;
		Xdp[j1] = Xrect[1];
	}
	else if( ( Ildp[j1] & IRIGHT ) != 0 ){
		Ydp[j1] = Ydp[j1] + (Xrect[2] - Xdp[j1])*dydp/dxdp;
		Xdp[j1] = Xrect[2];
	}

	/* -- Recompute the location of the "J1" point. */
	locdp( Xdp[j1], Ydp[j1], xrect, yrect, &Ildp[j1] );

	/* -- Return if both points are now inside rectangle. */
	if( Ildp[1] + Ildp[2] == 0 ){
		*ncdp = 2;
		goto L_8888;

	}
	/* -- Return if it can be determined that entire segment is outside. */
	else if( ( Ildp[1] & Ildp[2] ) != 0 ){
		*ncdp = 0;
		goto L_8888;
	}
	/* -- Otherwise continue the iterative clipping procedure. */
	else{
		goto L_2000;
	}
	
L_8888:
	return;

}

