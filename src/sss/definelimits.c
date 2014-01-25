
#include "sss.h"
#include "co.h"

void /*FUNCTION*/ definelimits(beginwindow, endwindow, begindata, 
	 enddata, delta, indexwindow, indexdata, numintersect)
double beginwindow, endwindow, begindata, enddata, delta;
int *indexwindow, *indexdata, *numintersect;
{
	int numdata, numstart, numstop;

	/*=====================================================================
	 * PURPOSE:  To define the limits of the intersection of a data file
	 *           and a time window.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *     beginwindow:  The beginning value of the window. [f]
	 *     endwindow:    The ending value of the window. [f]
	 *     begindata:    The beginning value of the data. [f]
	 *     enddata:      The ending value of the data. [f]
	 *     delta:        The sampling rate of the data. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *     indexwindow:  The index of the offset to the first point in
	 *                   the window.  This value should be added to
	 *                   the pointer to the first point in the window. [i]
	 *     indexdata:    The index of the offset to the first data point.
	 *     numintersect: The number of points in the intersection of the
	 *                   window and the data. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  sss/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    881117:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  881117
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Compute the offset in terms of number of points between beginning
	 *   of the window and the beginning of the data.
	 *   Round down to get the point at or before beginning of window. */
	numstart = (int)( (beginwindow - begindata)/delta );

	/* - Compute offset between end of the window and beginning of data.
	 *   Round up to get the point at or after end of window. */

	numstop = (int)( (endwindow - begindata)/delta ) + 1;

	/* - Compute number of points in the data set. */

	numdata = (int)( (enddata - begindata)/delta ) + 1;

	/* - Handle normal case where there is some overlap of window and data.
	 *   Compute starting indices and number of points in intersection. */

	if( numstart <= numdata && numstop >= 1 ){
		if( numstart >= 0 ){
			*indexwindow = 0;
			*indexdata = numstart;
			}
		else{
			*indexwindow = -numstart;
			*indexdata = 0;
			}
		*numintersect = min( numdata, numstop ) - *indexdata;

		/* - Handle special case where entire window is after data end 
		 *   or entire window before data begin (i.e. no data overlap.) */

		}
	else{
		*numintersect = 0;
		*indexwindow = 0;
		*indexdata = 0;
		}

       
	return;

} /* end of function */

