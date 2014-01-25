
#include <math.h>

#include "scm.h"
#include "bool.h"


#include "ucf.h"

void /*FUNCTION*/ xabsgl(data, nlen, thold, irgltp, nerr)
float data[];
int nlen;
double thold;
int irgltp, *nerr;
{
	int lgood;
	int ibad, igood, j;

	float *const Data = &data[0] - 1;


	/*=====================================================================
	 * PURPOSE: Remove glithces that exceed a certain value.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    data:    Array holding the data to be deglitched. [ra]
	 *    nlen:    Length of the data to be deglitched. [i]
	 *    thold:   Glitch threshold level. [r]
	 *    irgltp:  Method of smoothing. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  scm/3
	 *=====================================================================
	 * GLOBAL INPUT:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  linear, fill
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    lgood:   Flag to indicate whether a data point is in a
	 *             good or bad region. [l]
	 *    ibad:    Index of first bad point in window or region. [i]
	 *    nbad:    Number of bad points in data window. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    911223:  Split into a separate subroutine so that we can have
	 *             multiple methods of deglitching.
	 *    891025:  Added logic so that an active glitch region was 
	 *             terminated by the end of the data window.
	 *    890427:  Added ability to specifiy a time window in
	 *             which to apply glitch removal algorithm.
	 *    870206:  Original version from XSC RGL.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  890427
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* -- Search file for regions outside threshold.
	 *    Perform requested smoothing on data in each region. */
	lgood = TRUE;
	for( j = 1; j <= nlen; j++ ){
		if( lgood && fabs( Data[j] ) >= thold ){
			ibad = j;
			lgood = FALSE;
			}
		else if( !lgood && (fabs( Data[j] ) < thold || (j == nlen)) ){
			igood = j;
			if( irgltp == 1 ){
				linear( &Data[ibad - 1], igood - ibad + 2, &Data[ibad - 1] );
				}
			else if( irgltp == 2 ){
				fill( &Data[ibad], igood - ibad, 0. );
				}
			lgood = TRUE;
			}
		}


       
	return;

} /* end of function */

