
#include "mach.h"
#include "pl.h"
#include "gem.h"
#include "bool.h"
#include "sss.h"


#include "gtm.h"
#include "bot.h"
#include "gdm.h"

void plclip(xarray, yarray, number, lnewdp)
float xarray[], yarray[];
int number;
int lnewdp;
{
	int ltemp, lvisbl;
	int ildp[2], ildpc, j1, j2, j3, n, ncdp;
	float xblank, xcur, xpw[2], yblank, ycur, ypw[2];
	static int lblank = FALSE;

	int *const Ildp = &ildp[0] - 1;
	float *const Xarray = &xarray[0] - 1;
	float *const Xpw = &xpw[0] - 1;
	float *const Yarray = &yarray[0] - 1;
	float *const Ypw = &ypw[0] - 1;


	/*=====================================================================
	 * PURPOSE: To display a set of data points with clipping.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    xarray:  Array of x data points. [fa]
	 *    yarray:  Array of y data points. [fa]
	 *    number:  Number of x-y data points. [i]
	 *    lnewdp:  Set to .TRUE. if this is a new set of data points. [l]
	 *             Set to .FALSE. if a continuation of the last set.
	 *=====================================================================
	 * MODULE/LEVEL: gem/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    gem:     lline, icline, lsym, isym, isolid
	 *    sss:     lPlottingTT
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:     locdp, clipdp, polyline, setlinestyle, symbol
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    xpw:     Two element array containing x plot window. [f]
	 *    ypw:     Two element array containing y plot window. [f]
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
	 *    961004:  Added 3 lines to allow prs in origin right mode. maf
	 *    960829:  Added a check for NULL x values to support portrait mode
	 *    920526:  Added line-width.
	 *    920521:  Changed to allow non-display of specified NULL values.
	 *             (See the NULL command.)
	 *    910301:  Changed iline to icline.
	 *    900511:  Added calls to provide area blanking capability.
	 *    860113:  Fixed bug involving linestyle and symbol plotting.
	 *    830926:  Added logic to disable line and symbol plotting
	 *             if current attribute is zero.
	 *    830502:  Fixed bug involving symbols and linestyles.
	 *    820120:  Was not plotting symbol at first data point.
	 *    811223:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900511
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Copy plot window to local storage. */
	Xpw[1] = cmgem.uplot.xmin - VSMALL;
	Xpw[2] = cmgem.uplot.xmax + VSMALL;
	Ypw[1] = cmgem.uplot.ymin - VSMALL;
	Ypw[2] = cmgem.uplot.ymax + VSMALL;

	/* Next section added to allow prs in origin right mode. maf 961004 */
	  /* llefor became lOriginDefault.  maf 961004 */
	if ( cmsss.lPlottingTT && cmsss.lOriginDefault ) {
	    if ( Ypw[1] > Ypw[2] ) {
		float temp ;
		temp = Ypw[1] ;
		Ypw[1] = Ypw[2] ;
		Ypw[2] = temp ;
    	}
    	if ( Xpw[1] > Xpw[2] ) {
	        float temp ;
	        temp = Xpw[1] ;
	        Xpw[1] = Xpw[2] ;
	        Xpw[2] = temp ;
	    }
	}

	/* initialize */
	ildpc = 0 ;

	/* - Connect the line segments if requested. */

	if( cmgem.lline && cmgem.icline > 0 ){

		/* -- Locate the first data point relative to the rectangle. */
		locdp( Xarray[1], Yarray[1], xpw, ypw, &Ildp[1] );
		j1 = 1;

		for( j2 = 2; j2 <= number; j2++ ){

			/* -- Locate the current data point relative to the rectangle. */
			n = j2 - j1;
			j3 = 1;
			if( n > 0 )
				j3 = 2;
			locdp( Xarray[j2], Yarray[j2], xpw, ypw, &Ildp[j3] );

			/* -- Check for NULL y values. 
				added check for NULL x values for portrait mode of prs. maf 960829 */
			if( cmgem.lnull && ( (Yarray[j2] == cmgem.vnull) || (Xarray[j2] == cmgem.vnull) ) ) {
				if( n > 1 ){
					if( lblank ){
						plblank( (float*)&xblank, (float*)&yblank, 
						 &Xarray[j1], &Yarray[j1], n );
						}
					else{
						polyline( &Xarray[j1], &Yarray[j1], &n );
                                                stroke();
						}
					}
				j1 = j2 + 1;
				Ildp[2] = ildpc;
				}
			else{

				/* -- Only check for clip points if we already have more than one point */
				if( n > 0 ){

					/* -- If both locations are zero, the entire line segment is inside
					 *    the rectangle.  Do nothing until a current data point is outside. */

					/* -- If the logical intersection of both locations is non-zero, the
					 *    entire line segment is outside the rectangle.  Update start counter. */
					if( ( Ildp[1] & Ildp[2] ) != 0 ){
						j1 = j2;

						/* -- Otherwise, at least one of the data points in the line segment is
						 *    outside the rectangle.  Clip the line segment.  Plot the accumulated
						 *    line segments, if the current data point was outside. */
						}
					else if( Ildp[1] + Ildp[2] != 0 ){
						/* -- Save current data point values. */
						xcur = Xarray[j2];
						ycur = Yarray[j2];
						ildpc = Ildp[2];

						clipdp( &Xarray[j2 - 1], &Yarray[j2 - 1], 
						 ildp, xpw, ypw, &ncdp );
						if( ildpc != 0 ){
							n = n + 1;
							if( ncdp == 0 )
								n = n - 1;
							if( lblank ){
								plblank( (float*)&xblank, (float*)&yblank, 
								 &Xarray[j1], &Yarray[j1], n );
								}
							else{
								polyline( &Xarray[j1], &Yarray[j1], 
								 &n );
                                                                stroke();
								}
							j1 = j2;
							Xarray[j2] = xcur;
							Yarray[j2] = ycur;
							Ildp[2] = ildpc;
							}
						}
					}
				}

			if ( n > 0 ) Ildp[1] = Ildp[2];
			}

		/* -- Plot last set of contiguous line segments. */
		n = number - j1 + 1;
		if( n > 1 ){
			if( lblank ){
				plblank( (float*)&xblank, (float*)&yblank, &Xarray[j1], 
				 &Yarray[j1], n );
				}
			else{
				polyline( &Xarray[j1], &Yarray[j1], &n );
                                stroke();
				}
			}
		}

	/* - Label the data points if requested. */

	if( cmgem.lsym && cmgem.isym > 0 ){

		/* -- Symbols are always done using thin solid lines. */
		setlinestyle( LINE_STYLE_SOLID );
		setlinewidth( cmgem.isymwidth );
		ltemp = lnewdp;
		for( j2 = 1; j2 <= number; j2++ ){

			/* -- See if data point is inside plot window. */
			locdp( Xarray[j2], Yarray[j2], xpw, ypw, &Ildp[1] );
			lvisbl = Ildp[1] == 0;

			/* -- See if data point is outside area blanking region(s). */
			if( lvisbl && lblank ){
				locdp( Xarray[j2], Yarray[j2], (float*)&xblank, (float*)&yblank, 
				 &Ildp[1] );
				lvisbl = Ildp[1] != 0;
				}

			/* -- Render any visible data points. */
			if( lvisbl )
				symbol( &Xarray[j2], &Yarray[j2], 1, ltemp );

			ltemp = FALSE;
			}

		/* -- Restore current linestyle and thickness. */
		setlinestyle( cmgem.icline );
		setlinewidth( cmgem.iwidth );
		}

       
	return;
} /* end of function */

