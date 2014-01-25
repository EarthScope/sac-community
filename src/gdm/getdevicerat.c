
#include "mach.h"
#include "gdm.h"
#include "co.h"

/** 
 * Get the Graphics device aspect ratio
 *
 * @param ratio
 *   Aspect ratio of currently active graphics device 
 *
 * @date   870217:  Changed name from getscreenatr.
 * @date   861026:  Original version.
 *
 */
void 
getdeviceratio(float *ratio)
{
	float ratio2;

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	/* - Determine the minimum screen aspect (y to x) ratio of all active
	 *   graphics devices. */
	*ratio = VLARGE;
        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->get_device_ratio) {
            dev[i]->get_device_ratio( &ratio2 );
            *ratio = fmin( *ratio, ratio2 );
          }
        }
	if( *ratio == VLARGE ) {
          *ratio = 1.0;
        }

}

