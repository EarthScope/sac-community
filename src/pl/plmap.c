
#include <math.h>

#include "mach.h"
#include "pl.h"
#include "gem.h"
#include "co.h"

#include "gtm.h"
#include "ucf.h"

double
yinterp(double x, double x0, double x1, double y0, double y1) {
  return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}

#define IDX2TIME( id ) (cmgem.xgen.first + (id-1) * cmgem.xgen.delta)

void /*FUNCTION*/ plmap(xarray, yarray, number, incx, incy, nerr)
float xarray[], yarray[];
int number, incx, incy, *nerr;
{
	int jdx, jx, jy, num1, num2, num2m1;
	float delx, dely, fjunk, tmp, xvspdl, yvspdl;

	float *const Xarray = &xarray[0] - 1;
	float *const Yarray = &yarray[0] - 1;


	/*=====================================================================
	 * PURPOSE:  To perform mapping from input to plot coordinate systems.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    xarray:  X data array. [f]
	 *    yarray:  Y data array. [f]
	 *    number:  Number of data pairs to plot. [i]
	 *    incx:    Increment in XARRAY array between data points. [i]
	 *    incy:    Increment in YARRAY array. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred. [i]
	 *             Potential error numbers:  NONE
	 *=====================================================================
	 * MODULE/LEVEL:  gem/3
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    vlarge, vsmall
	 *    gem:     lxlim, ximn, ximx, lylim, yimn, yimx, yvspmx
	 *             lxgen, xfirst, xdelta, lygen, yfirst, ydelta,
	 *             ximn, ximx, yimn, yimx, ixint, iyint, ilin, ilog,
	 *             lxfudg, lyfudg, lxrev, lyrev, lxmpiw, lympiw
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gem:     ximnu, ximxu, yimnu, yimxu,
	 *             ximnz, ximxz, yimnz, yimxz, xpmnu, xpmxu, ypmnu, ypmxu,
	 *             xmpip1, xmpip2, ympip1, ympip2
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:     extrma, plcalwvtrans, setvport, setworld
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900306:  Moved mapping transformation from here to plcalwvtrans.
	 *             Added calls to set viewport and world coordinates.
	 *    870602:  Added coding to scale plot window to current viewspace.
	 *    870112:  Deleted coding to reverse plot window if limits reversed.
	 *    860404:  Made floor for log plots a fraction of maximum.
	 *    860213:  Deleted lnice logic from x and y limit calculations.
	 *    841218:  Fixed bug involving log mapping with non-positive limits.
	 *    831103:  Added scaling for viewspace ratio.
	 *    830929:  Deleted extra mapping from "input" to "world" coordinates.
	 *    830811:  Replaced call to zmnmxr with do-loop.
	 *    810000:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870112
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - X limits are either fixed or are scaled to the input data. */

	if( cmgem.lxlim ){
	    cmgem.data.xmin = cmgem.ximn;
	    cmgem.data.xmax = cmgem.ximx;
	}
	else{
	    if( cmgem.xgen.on ){
		cmgem.data.xmin = cmgem.xgen.first;
		cmgem.data.xmax = cmgem.xgen.first + (number - 1)*cmgem.xgen.delta;
	    }
	    else
		extrma( xarray, incx, number, &cmgem.data.xmin, &cmgem.data.xmax, &fjunk );
	}

	/* - Y limits are a bit more complicated:
	 *   (1) They can have fixed values.
	 *   (2) They can be scaled to the entire y array if the x data is also scaled.
	 *   (3) They can be scaled to that portion of the y array, where the
	 *       corresponding x value is within the fixed x range.
	 *       This subcase itself has two subcases (getting confused?):
	 *       (3a) X data array present; range checked extrema calculation used.
	 *       (3b) X data to be generated; extrma used but the range of the y
	 *            array search is confined to the equivalent x data limits. */

	if( cmgem.lylim ){
	    cmgem.data.ymin = cmgem.yimn;
	    cmgem.data.ymax = cmgem.yimx;
	}
	else{
	    if( cmgem.ygen.on ){
		cmgem.data.ymin = cmgem.ygen.first;
		cmgem.data.ymax = cmgem.ygen.first + (number - 1)*cmgem.ygen.delta;
	    }
	    else{
        if( cmgem.lxlim ){
          if( cmgem.xgen.on ){
            num1 = (int)( ceil((cmgem.data.xmin - cmgem.xgen.first)/cmgem.xgen.delta) ) + 1;
            if( num1 < 1 )
              num1 = 1;
            num2 = (int)( floor((cmgem.data.xmax - cmgem.xgen.first)/cmgem.xgen.delta) ) + 1;
            if( num2 > number )
              num2 = number;
            if( num1 <= number && num2 >= 1 ){
              num2m1 = num2 - num1 + 1;
              extrma( &Yarray[num1], incy, num2m1, &cmgem.data.ymin, 
                      &cmgem.data.ymax, &fjunk );
              /* Check points just outside of the plot region */
              if(num2 < number && Yarray[num2+1] >= cmgem.data.ymax) {
                cmgem.data.ymax = yinterp(cmgem.data.xmax, 
                                          IDX2TIME(num2), IDX2TIME(num2+1),
                                          Yarray[num2], Yarray[num2+1]);
              }
              if(num2 < number && Yarray[num2+1] <= cmgem.data.ymin) {
                cmgem.data.ymin = yinterp(cmgem.data.xmax, 
                                          IDX2TIME(num2), IDX2TIME(num2+1),
                                          Yarray[num2], Yarray[num2+1]);
              }
              if(num1 > 1 && Yarray[num1-1] >= cmgem.data.ymax) {
                cmgem.data.ymax = yinterp(cmgem.data.xmin, 
                                          IDX2TIME(num1-1), IDX2TIME(num1),
                                          Yarray[num1-1], Yarray[num1]);
              }
              if(num1 > 1 && Yarray[num1-1] <= cmgem.data.ymin) {
                cmgem.data.ymin = yinterp(cmgem.data.xmin, 
                                          IDX2TIME(num1-1), IDX2TIME(num1),
                                          Yarray[num1-1], Yarray[num1]);
              }
            }
            else{
              cmgem.data.ymin = -1.;
              cmgem.data.ymax = 1.;
            }
          } /* end if( cmgem.lxgen ) */
          else{
			jx = 1;
			jy = 1;
			cmgem.data.ymax = -VLARGE;
			cmgem.data.ymin = VLARGE;
			for( jdx = 1; jdx <= number; jdx++ ){
			    if( Xarray[jx] >= cmgem.data.xmin && Xarray[jx] <= cmgem.data.xmax ){
				if( Yarray[jy] < cmgem.data.ymin )
				    cmgem.data.ymin = Yarray[jy];
				if( Yarray[jy] > cmgem.data.ymax )
				    cmgem.data.ymax = Yarray[jy];
			    }
			    jx = jx + incx;
			    jy = jy + incy;
			} /* end for ( jdx ) */
		    } /* end else associated with if( cmgem.lxgen ) */
		} /* end if( cmgem.lxlim ) */
		else
		    extrma( yarray, incy, number, &cmgem.data.ymin, &cmgem.data.ymax, &fjunk );
	    } /* end else associated with if( cmgem.lygen ) */
	} /* end else associated with if( cmgem.lylim ) */

	/* - Adjust data limits to produce 'nice' plots if limits not fixed.
	 * - If LXFUDG or LYFUDG is .TRUE. make that data window
	 *   slightly larger than extrema.
	 * - If logarithmic:
	 *   (1) Make sure data limits are positive.
	 *   (2) Change data limits to log10 of data limits.
	 *   (3) If LXFULL or LYFULL is true, make data limits whole decades. */

	if( cmgem.ixint == AXIS_LINEAR ){
	    cmgem.zdata.xmin = cmgem.data.xmin;
	    cmgem.zdata.xmax = cmgem.data.xmax;
	    if( cmgem.lxfudg && !cmgem.lxlim ){
		delx = cmgem.xfudg*(cmgem.zdata.xmax - cmgem.zdata.xmin);
		if( delx < VSMALL )
		    delx = 0.00001*fabs( cmgem.zdata.xmin );
		cmgem.zdata.xmin = cmgem.zdata.xmin - delx;
		cmgem.zdata.xmax = cmgem.zdata.xmax + delx;
	    }
	}
	else if( cmgem.ixint == AXIS_LOG ){
	    if( cmgem.data.xmax <= 0. )
		cmgem.data.xmax = 1.;
	    cmgem.zdata.xmax = log10( cmgem.data.xmax );
	    if( cmgem.data.xmin <= 0. )
		cmgem.data.xmin = fmax( cmgem.floor*cmgem.data.xmax, VSMALL );
	    cmgem.zdata.xmin = log10( cmgem.data.xmin );
	    if( !cmgem.lxfull ){
		if( cmgem.lxfudg ){
		    delx = cmgem.xfudg*(cmgem.zdata.xmax - cmgem.zdata.xmin);
		    cmgem.zdata.xmin = cmgem.zdata.xmin - delx;
		    cmgem.zdata.xmax = cmgem.zdata.xmax + delx;
		}
		if( cmgem.lxfull ){
		    tmp = cmgem.zdata.xmin;
		    if( tmp < 0. )
			tmp = tmp - 0.99999;
		    cmgem.zdata.xmin = (float)( (int)( tmp ) );
		    tmp = cmgem.zdata.xmax;
		    if( tmp > 0. )
			tmp = tmp + 0.99999;
		    cmgem.zdata.xmax = (float)( (int)( tmp ) );
		} /* end if( cmgem.lxfull ) */
	    } /* end if( !cmgem.lxfull ) */
	} /* end else if( cmgem.ixint == AXIS_LOG ) */

	if( cmgem.iyint == AXIS_LINEAR ){
	    cmgem.zdata.ymin = cmgem.data.ymin;
	    cmgem.zdata.ymax = cmgem.data.ymax;
	    if( cmgem.lyfudg && !cmgem.lylim ){
		dely = cmgem.yfudg*(cmgem.zdata.ymax - cmgem.zdata.ymin);
		if( dely < VSMALL )
		    dely = 0.00001*fabs( cmgem.zdata.ymin );
		cmgem.zdata.ymin = cmgem.zdata.ymin - dely;
		cmgem.zdata.ymax = cmgem.zdata.ymax + dely;
	    }
	} /* end if( cmgem.iyint == AXIS_LINEAR ) */
	else if( cmgem.iyint == AXIS_LOG ){
	    if( cmgem.data.ymax <= 0. )
		cmgem.data.ymax = 1.;
	    cmgem.zdata.ymax = log10( cmgem.data.ymax );
	    if( cmgem.data.ymin <= 0. )
		cmgem.data.ymin = fmax( cmgem.floor*cmgem.data.ymax, VSMALL );
	    cmgem.zdata.ymin = log10( cmgem.data.ymin );
	    if( !cmgem.lylim ){
		if( cmgem.lyfudg ){
		    dely = cmgem.yfudg*(cmgem.zdata.ymax - cmgem.zdata.ymin);
		    cmgem.zdata.ymin = cmgem.zdata.ymin - dely;
		    cmgem.zdata.ymax = cmgem.zdata.ymax + dely;
		}
		if( cmgem.lyfull ){
		    tmp = cmgem.zdata.ymin;
		    if( tmp < 0. )
			tmp = tmp - 0.99999;
		    cmgem.zdata.ymin = (float)( (int)( tmp ) );
		    tmp = cmgem.zdata.ymax;
		    if( tmp > 0. )
			tmp = tmp + 0.99999;
		    cmgem.zdata.ymax = (float)( (int)( tmp ) );
		} /* end if( cmgem.lyfull ) */
	    } /* end if( !cmgem.lylim ) */
	} /* end else if( cmgem.iyint == AXIS_LOG ) */

	/* - Map plot window to current viewspace. */

	xvspdl = cmgem.view.xmax - cmgem.view.xmin;
	cmgem.uplot.xmin = cmgem.view.xmin + cmgem.plot.xmin * xvspdl;
	cmgem.uplot.xmax = cmgem.view.xmin + cmgem.plot.xmax * xvspdl;

	yvspdl = cmgem.view.ymax - cmgem.view.ymin;
	cmgem.uplot.ymin = cmgem.view.ymin + cmgem.plot.ymin * yvspdl;
	cmgem.uplot.ymax = cmgem.view.ymin + cmgem.plot.ymax * yvspdl;

	/* - Reverse either plot window. */

	if( cmgem.lxrev ){
	    tmp = cmgem.uplot.xmin;
	    cmgem.uplot.xmin = cmgem.uplot.xmax;
	    cmgem.uplot.xmax = tmp;
	}

	if( cmgem.lyrev ){
	    tmp = cmgem.uplot.ymin;
	    cmgem.uplot.ymin = cmgem.uplot.ymax;
	    cmgem.uplot.ymax = tmp;
	}

	/* - If XIMNZ eq XIMXZ, make XIMXZ slightly larger. */

	if( cmgem.zdata.xmin == cmgem.zdata.xmax ){
	    if( cmgem.zdata.xmin == 0. ){
		cmgem.zdata.xmax = 0.000001;
	    }
	    else{
		cmgem.zdata.xmax = cmgem.zdata.xmin + 0.1*cmgem.zdata.xmax;
	    }
	}

	/* - Do the same for the y data limits. */

	if( cmgem.zdata.ymin == cmgem.zdata.ymax ){
	    if( cmgem.zdata.ymin == 0. ){
		cmgem.zdata.ymax = 0.000001;
	    }
	    else{
		cmgem.zdata.ymax = cmgem.zdata.ymin + 0.1*cmgem.zdata.ymax;
	    }
	}

	/* - Calculate the input to plot coordinate mapping transformation. */

	plcalwvtrans();

	/* - Set viewport and world limits.
	 ***NOT CURRENTLY BEING USED TO DO MAPPING. STILL IN TRANSITION*** */

	setvport( cmgem.uplot.xmin, cmgem.uplot.xmax, 
                  cmgem.uplot.ymin, cmgem.uplot.ymax );
	setworld( cmgem.zdata.xmin, cmgem.zdata.xmax, cmgem.zdata.ymin, cmgem.zdata.ymax );

} /* end of function */

