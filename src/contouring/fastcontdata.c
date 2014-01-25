/** 
 * @file   fastcontdata.c
 * 
 * @brief  Fast Contour Plot
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mach.h"
#include "gtm.h"
#include "contouring.h"
#include "co.h"

#include "gdm.h"

void 
fastcontdata(array, nxsize, nysize, nerr)
float array[];
int nxsize, nysize, *nerr;
{
	int highright[4], storemode;
	int icolorlist, ihigh, ilow, inum, jlevel, jx, 
	 jxstart, jxstop, jy, jystart, jystop;
	float a0, a1, a2, a3, a4, a5, a6, a7, delx, dely, dmax, dmin, 
	 pos[4][2], test, xbase, ybase, zll, zlr, zmax, zmin, zul, zur;
        float lmax, lmin, umax, umin;

	float *const Array = &array[0] - 1;
	int *const Highright = &highright[0] - 1;


	/*=====================================================================
	 * PURPOSE:  To do a fast contour plot with limited options.
	 *           Only solid linestyle, no labeling, no tickmarks, etc.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *      array:  Two-dimensional array of data. [fa]  
	 *     nxsize:  Number of elements in the x (horizontal) direction. [i]
	 *     nysize:  Number of elements in the y (vertical) direction. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error return flag.  Set to 0 if no error occurred. [i]
	 *=====================================================================
	 * MODULE/LEVEL: contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:     
	 *    gtm:         xvpmin, xvpmax, yvpmin, yvpmax
	 *    contouring:  ixdatastart, ixdatastop, iydatastart, iydatastop,
	 *                 kcolormode, ncolorlist, kcolorlist
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *     saclib:  move, draw, setcolorname
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900430:  Added color capability.
	 *    900409:  Added ability to contour subset of data.
	 *    900321:  Added logic to make direction of line segments such that
	 *             the z values are always higher to the right of segment.
	 *    900317:  Original version from subroutine by Dave Harris.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900321
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Set up data plot limits. */

	if( cmcontouring.ixdatastart <= 0 ){
		jxstart = 1;
		}
	else{
		jxstart = cmcontouring.ixdatastart;
		}
	if( cmcontouring.ixdatastop <= 0 ){
		jxstop = nxsize - 1;
		}
	else{
		jxstop = cmcontouring.ixdatastop - 1;
		}

	if( cmcontouring.iydatastart <= 0 ){
		jystart = 1;
		}
	else{
		jystart = cmcontouring.iydatastart;
		}
	if( cmcontouring.iydatastop <= 0 ){
		jystop = nysize - 1;
		}
	else{
		jystop = cmcontouring.iydatastop - 1;
		}

	/* - Set up scaling and offsets for plotting. */

	delx = (cmgtm.xvpmax - cmgtm.xvpmin)/(float)( jxstop - jxstart + 
	 1 );
	dely = (cmgtm.yvpmax - cmgtm.yvpmin)/(float)( jystop - jystart + 
	 1 );
	zmin = Zlevels[1];
	zmax = Zlevels[cmcontouring.nzlevels];

	/* - Main calculation loop: */

	storemode = strcmp(kmcontouring.kdatamode,"ROWS    ") == 0;
	for( jy = jystart; jy <= jystop; jy++ ){
		for( jx = jxstart; jx <= jxstop; jx++ ){

			/* -- Define corners of current plotting square. */
			if( storemode ){
				zll = Array[jx + (jy - 1)*nxsize];
				zlr = Array[jx + 1 + (jy - 1)*nxsize];
				zul = Array[jx + jy*nxsize];
				zur = Array[jx + 1 + jy*nxsize];
				}
			else{
				zll = Array[jy + (jx - 1)*nysize];
				zlr = Array[jy + 1 + (jx - 1)*nysize];
				zul = Array[jy + jx*nysize];
				zur = Array[jy + 1 + jx*nysize];
				}

			/* -- Determine if any contour intersects the square. */

                        lmax = fmax(zll,zlr);
                        umax = fmax(zul,zur);
                        dmax = fmax(lmax,umax);

                        lmin = fmin(zll,zlr);
                        umin = fmin(zul,zur);
                        dmin = fmin(lmin,umin);

			/* -- If no intersection continue on to next square. */
			if( (dmax < zmin) || (dmin > zmax) )
				goto L_4000;

			/* -- If all four corners of square have same value skip this square. */
			if( dmax == dmin )
				goto L_4000;

			/* -- Determine number of intersecting contours. */
			ilow = 1;
			ihigh = cmcontouring.nzlevels;
			for( jlevel = 1; jlevel <= cmcontouring.nzlevels; jlevel++ ){
				if( Zlevels[jlevel] <= dmin ){
					ilow = jlevel + 1;
					}
				else if( Zlevels[jlevel] >= dmax ){
					ihigh = jlevel - 1;
					goto L_2100;
					}
				}
L_2100:
			;

			/* -- Locate intersections of contours with boundaries of square. */
			for( jlevel = ilow; jlevel <= ihigh; jlevel++ ){
				inum = 0;
				test = Zlevels[jlevel];
				xbase = (float)( jx - jxstart )*delx + cmgtm.xvpmin;
				ybase = (float)( jy - jystart )*dely + cmgtm.yvpmin;
				a0 = fmin( zll, zlr );
				a1 = fmax( zll, zlr );
				a2 = fmin( zul, zur );
				a3 = fmax( zul, zur );
				a4 = fmin( zll, zul );
				a5 = fmax( zll, zul );
				a6 = fmin( zlr, zur );
				a7 = fmax( zlr, zur );

				/* -- Set color if requested. */
				if( strcmp(kmcontouring.kcolormode,"ON      ") == 
				 0 ){
					icolorlist = jlevel%cmcontouring.ncolorlist;
					if( icolorlist == 0 )
						icolorlist = cmcontouring.ncolorlist;
					setcolorname( (char*)kmcontouring.kcolorlist[icolorlist - 1]
					 ,9 );
					}

				if( (test > a0) && (test < a1) ){
					inum = inum + 1;
					if( zlr != zll ){
						pos[inum - 1][0] = xbase + (test - zll)/(zlr - 
						 zll)*delx;
						}
					else{
						pos[inum - 1][0] = xbase;
						}
					Highright[inum] = zlr >= test;
					pos[inum - 1][1] = ybase;
					}

				if( (test > a2) && (test < a3) ){
					inum = inum + 1;
					if( zur != zul ){
						pos[inum - 1][0] = xbase + (test - zul)/(zur - 
						 zul)*delx;
						}
					else{
						pos[inum - 1][0] = xbase;
						}
					Highright[inum] = zul >= test;
					pos[inum - 1][1] = ybase + dely;
					}

				if( (test > a4) && (test < a5) ){
					inum = inum + 1;
					pos[inum - 1][0] = xbase;
					if( zul != zll ){
						pos[inum - 1][1] = ybase + (test - zll)/(zul - 
						 zll)*dely;
						}
					else{
						pos[inum - 1][1] = ybase;
						}
					Highright[inum] = zll >= test;
					}

				if( (test > a6) && (test < a7) ){
					inum = inum + 1;
					pos[inum - 1][0] = xbase + delx;
					if( zur != zlr ){
						pos[inum - 1][1] = ybase + (test - zlr)/(zur - 
						 zlr)*dely;
						}
					else{
						pos[inum - 1][1] = ybase;
						}
					Highright[inum] = zur >= test;
					}

				/* -- Draw these line segments. */
				if( inum == 2 ){
					move( pos[0][0], pos[0][1] );
					draw( pos[1][0], pos[1][1] );
					}
				else if( inum == 4 ){
					if( Highright[1] ){
						move( pos[0][0], pos[0][1] );
						draw( pos[3][0], pos[3][1] );
						}
					else{
						move( pos[2][0], pos[2][1] );
						draw( pos[0][0], pos[0][1] );
						}
					if( Highright[2] ){
						move( pos[1][0], pos[1][1] );
						draw( pos[3][0], pos[3][1] );
						}
					else{
						move( pos[2][0], pos[2][1] );
						draw( pos[1][0], pos[1][1] );
						}
					}
				}
L_4000:
			;
			}
		}

	/* - Reset color attribute if necessary. */

	if( strcmp(kmcontouring.kcolormode,"ON      ") == 0 )
		setcolorname( "DEFAULT",8 );

	return;
}

