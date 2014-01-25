
#include "gdm.h"

/** 
 * Get the geometry of the active window
 *
 * @param width_return
 *    Width of active window
 * @param height_return
 *    Height of active window
 * @param nerr
 *    Error return flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date 940823: Original version.
 *
 */
void 
get_geometry(unsigned int *width_return,
             unsigned int *height_return,
             int *nerr)
{

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->get_geometry) {
            dev[i]->get_geometry(cmgdm.iwindow, width_return, height_return, nerr);
          }
        }

	return;

}

