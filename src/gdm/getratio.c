
#include "mach.h"
#include "gdm.h"
#include "co.h"

/** 
 * Get the current viewspace aspect ratio
 *
 * @param ratio
 *    Minimum aspect ratio of all graphics devices
 *
 * @date   861026:  Original version.
 *
 */
void 
getratio(float *ratio)
{
	float ratio2;

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	/* - Determine the minimum viewspace (y to x) ratio of all active
	 *   graphics devices. */
	*ratio = VLARGE;
        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->get_ratio) {
            dev[i]->get_ratio( &ratio2 );
            *ratio = fmin( *ratio, ratio2 );
          }
        }

	if( *ratio == VLARGE ) {
          *ratio = 1.0;
        }
}

