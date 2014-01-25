
#include "gdm.h"
#include "bool.h"

/** 
 * Get attributes of a graphics window
 *
 * @param nwidnow
 *   Graphics window number
 * @param exists
 *   - TRUE if window exists
 *   - FALSE if window does not exist
 *
 * @date   870217:  Changed name form getwindowatr.
 * @date   870127:  Original version.
 *
 */
void 
getwindowstatus(int *nwindow, 
                int *exists)
{
        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	*exists = FALSE;

	/* - Inquire about the existence of the requested graphics window. */
        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->get_window_status) {
            dev[i]->get_window_status( *nwindow, exists );
          }
        }

}

