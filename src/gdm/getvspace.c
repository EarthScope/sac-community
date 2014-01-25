
#include "gdm.h"

/** 
 * Get current viewspace range
 *
 * @param xvsmin
 *    Minimum X Viewspace value
 * @param xvsmax
 *    Maximum X Viewspace value
 * @param yvsmin
 *    Minimum Y Viewspace value
 * @param yvsmax
 *    Maximum Y Viewspace value
 *
 * @date   861021:  Original version.
 *
 */
void 
getvspace(float *xvsmin,
          float *xvsmax,
          float *yvsmin,
          float *yvsmax)
{
	/* - Report current common block values. */
	*xvsmin = Xvs[1];
	*xvsmax = Xvs[2];
	*yvsmin = Yvs[1];
	*yvsmax = Yvs[2];
}

