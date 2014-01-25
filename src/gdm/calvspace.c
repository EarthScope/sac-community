
#include <stdio.h>

#include "gdm.h"

/** 
 * Calculate the current graphics viewspace
 *
 * @date   861020:  Major restructuring.
 * @date   840911:  Major modifications to allow for any viewspace ratio.
 *                  Moved from dependent to independent sources.
 * @date   831026:  Original version.
 */
void 
calvspace(void)
{
	float vsdes, vsdev;

	/* - Determine the minimum viewspace (y to x) ratio of all active
	 *   graphics devices. */
	getratio( &vsdev );

	/* - Determine desired viewspace ratio.  This is either a the
	 *   viewspace value calculated above or a specific requested ratio. */
	if( cmgdm.lvsful ){
          vsdes = vsdev;
        } else {
          vsdes = cmgdm.vsrat;
        }

	/* - Calculate the viewspace limits so that the desired viewspace
	 *   will fit onto all of the active graphics devices. */
	if( vsdes <= vsdev ){
          Xvs[1] = 0.;
          Xvs[2] = 1.;
          Yvs[1] = 0.;
          Yvs[2] = vsdes;
        } else {
          Xvs[1] = 0.5*(1.0 - vsdev/vsdes);
          Xvs[2] = 1.0 - Xvs[1];
          Yvs[1] = 0.;
          Yvs[2] = vsdev;
        }

} 

