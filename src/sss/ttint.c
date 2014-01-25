
#include <math.h>

#include "sss.h"
#include "gem.h"

void ttint(darray, tarray, npts, dvint, tvint, nerr)
float darray[], tarray[];
int npts;
float *dvint, *tvint;
int *nerr;
{
	int idx, ndx ;
	float del, dwint, twint;

	float *const Darray = &darray[0] - 1;
	float *const Tarray = &tarray[0] - 1;


	/*=====================================================================
	 * PURPOSE:  Find where the traveltime curve intercepts the time axis.
	 *           For now assume that indeed the travel time  curve does.
	 *           In future find the max valid time value.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *     npts:     Number of points in this line. (i)
	 *     darray:   Array of d values for this line. (fa)
	 *     tarray:   Array of t values for this line. (fa)
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    dvint:   d-axis intercept in viewport cordinates.
	 *    tvint:   t-axis intercept in viewport cordinates.
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  sss/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    twint:   t-axis intercept in world cordinates.
	 *    dwint:   d-axis intercept in world cordinates.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    960829:  Overhauled to allow portrait mode compatability.
         *             Every thing was changed from the perspective of
	 *             the X and Y axis to the perspective of the distance (d)
	 *             axis and the time (t) axis.
	 *    920729:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;
	*tvint = -1.0;
	if ( cmsss.lorient )
	    dwint = cmgem.yimx ;
	else
	    dwint = cmgem.ximx;

	/* - Compute time */

	if( cmgem.xgen.on ){
	    ndx = (dwint - cmgem.xgen.first)/cmgem.xgen.delta;
	    del = fabs( dwint - (float)( ndx )*cmgem.xgen.delta + cmgem.xgen.first );
	    ndx = ndx + 1;
	    if( Tarray[ndx] == -1.0 ){
		twint = -1.0;
	    }
	    else{
		if( del < RNDOFF ){
			twint = Tarray[ndx];
		}
		else{
		    twint = (Tarray[ndx + 1] - Tarray[ndx])/cmgem.xgen.delta*del + Tarray[ndx];
		}
	    }
	}
	else{
	    for( ndx = 1; ndx <= npts; ndx++ ){
		if( fabs( Darray[ndx] - dwint ) < RNDOFF ) {
		    twint = Tarray[ndx];
		    goto L_50;
		}
		if( (ndx < npts && Darray[ndx] <= dwint) && Darray[ndx + 1] > 
		 dwint ) {
		    if( Tarray[ndx] == -1.0 ){
			twint = -1.0;
		    }
		    else{
			twint = ( Tarray[ ndx + 1 ] - Tarray[ ndx ] ) /
				( Darray[ ndx + 1 ] - Darray[ ndx ] ) *
				( dwint - Darray[ ndx ] ) + Tarray[ ndx ] ;
		    }
		    goto L_50;
		}
	    }
	    /* if twint not found, set it to max valid time 
	    twint = -Tarray[ 1 ] ;
	    for ( ndx = 2 ; ndx <= npts ; ndx++ ) {
		if ( twint < Tarray[ ndx ] )
		    twint = Tarray[ ndx ] ;
	    }  */
	    goto L_8888 ;
	}
L_50:
	if( twint == -1.0 ){
	    for( idx = ndx; idx >= 1; idx-- ){
		twint = Tarray[idx];
		if( twint != -1.0 ){
		    if( cmgem.xgen.on ){
			dwint = cmgem.xgen.first + (idx - 1)*cmgem.xgen.delta;
		    }
		    else{
			dwint = Darray[idx];
		    }
		    goto L_60;
		}
	    }
	    goto L_8888;
	}
L_60:

	if ( cmsss.lorient ) {
            *dvint = cmgem.ympip1*dwint + cmgem.ympip2;
            *tvint = cmgem.xmpip1*twint + cmgem.xmpip2;
	}
	else { 
	    *dvint = cmgem.xmpip1*dwint + cmgem.xmpip2;
	    *tvint = cmgem.ympip1*twint + cmgem.ympip2;
	}


L_8888:
	return;

} /* end of function */

