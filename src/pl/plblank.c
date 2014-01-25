
#include "pl.h"
#include "gem.h"


#include "gtm.h"
#include "bot.h"

void /*FUNCTION*/ plblank(xblank, yblank, xarray, yarray, number)
float xblank[], yblank[], xarray[], yarray[];
int number;
{
	int ildp[2], itemp[2], j1, j2, n, ncdp;
	float xcur, xtemp[2], ycur, ytemp[2];

	int *const Ildp = &ildp[0] - 1;
	int *const Itemp = &itemp[0] - 1;
	float *const Xarray = &xarray[0] - 1;
	float *const Xtemp = &xtemp[0] - 1;
	float *const Yarray = &yarray[0] - 1;
	float *const Ytemp = &ytemp[0] - 1;


	/*=====================================================================
	 * PURPOSE: To display a set of data points with area blanking.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    xblank:  Array of x blanking area bounds. [fa=2]
	 *             First value is left edge. Second is right edge.
	 *    yblank:  Array of y blanking area bounds. [fa=2]
	 *             First value is bottom edge. Second is top edge.
	 *    xarray:  Array of x data points. [fa]
	 *    yarray:  Array of y data points. [fa]
	 *    number:  Number of x-y data points. [i]
	 *=====================================================================
	 * MODULE/LEVEL: gem/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    gem:     lline, icline, lsym, isym
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:     locdp, clipdp, polyline, setlinestyle, symbol
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    ildp:    Two element array giving location of last and current
	 *             data point relative to plot window.  See LOCDP
	 *             documentation for exact definitions. [i]
	 *    xcur:    Current x data value.  Saved in local storage since
	 *             clipping algorithm (CLIPDP) may change input arrays. [f]
	 *    ycur:    Current y data value. [f]
	 *    ildpc:   Value of ILDP for current data point. [i]
	 *    lvisbl:  Set to .TRUE. if current data point is visible. [l]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    910301:  Changed iline to icline.
	 *    900511:  Original version based on modified plclip subroutine.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900511
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Connect the line segments if requested. */
	if( cmgem.lline && cmgem.icline > 0 ){

		/* -- Locate the first data point relative to the rectangle. */
		locdp( Xarray[1], Yarray[1], xblank, yblank, &Ildp[1] );
		j1 = 1;

		for( j2 = 2; j2 <= number; j2++ ){
	
			/* -- Locate the current data point relative to the rectangle. */
			locdp( Xarray[j2], Yarray[j2], xblank, yblank, &Ildp[2] );

			/* -- Save current data point values. */
			xcur = Xarray[j2];
			ycur = Yarray[j2];

			/* -- If the logical intersection of both locations is non-zero, the
			 *    entire line segment is outside the rectangle.  
			 *    Do nothing until a current point is outside. */
			if( ( Ildp[1] & Ildp[2] ) != 0 ){

				/* -- If both locations are zero, the entire line segment is inside
				 *    the rectangle.  Update the starting counter. */
				}
			else if( Ildp[1] + Ildp[2] == 0 ){
				j1 = j2;

				/* -- Otherwise, at least one of the data points in the line segment is
				 *    inside the rectangle.  Use the clipping algorithm to clip the line 
				 *    segment to the inside of the area BUT PLOT THE PART OF THE LINE
				 *    SEGMENT THAT IS OUTSIDE THE AREA. */
				}
			else{
				Xtemp[1] = Xarray[j2 - 1];
				Ytemp[1] = Yarray[j2 - 1];
				Xtemp[2] = Xarray[j2];
				Ytemp[2] = Yarray[j2];
				Itemp[1] = Ildp[1];
				Itemp[2] = Ildp[2];
				clipdp( xtemp, ytemp, itemp, xblank, yblank, &ncdp );
				if( Ildp[1] != 0 ){
					Xarray[j2] = Xtemp[1];
					Yarray[j2] = Ytemp[1];
					n = j2 - j1 + 1;
					if( ncdp == 0 )
						n = n - 1;
					polyline( &Xarray[j1], &Yarray[j1], &n );
					j1 = j2;
					Xarray[j2] = xcur;
					Yarray[j2] = ycur;
					Ildp[2] = 0;
					}
				if( Ildp[2] != 0 ){
					j1 = j2 - 1;
					Xarray[j2 - 1] = Xtemp[2];
					Yarray[j2 - 1] = Ytemp[2];
					Ildp[1] = Itemp[2];
					}
				}
			Ildp[1] = Ildp[2];
			}

		/* -- Plot last set of contiguous line segments. */
		n = number - j1 + 1;
		if( n > 1 )
			polyline( &Xarray[j1], &Yarray[j1], &n );
                        stroke();
		}

	return;

} /* end of function */

