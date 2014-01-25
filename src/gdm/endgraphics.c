
#include "gdm.h"
#include "bool.h"

/** 
 * End the graphics module library
 *
 * Terminate each active graphics device
 *
 * @param nerr
 *   Error return flag
 *   - 0 on Success
 *   - Non-Zero on Error
 *
 * @date   861010:  Original version.
 *
 */
void 
endgraphics(int *nerr)
{
        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	*nerr = 0;

	/* - Terminate each active graphic device. */
        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->end_device) {
            dev[i]->end_device( nerr );
          }
        }

	/* - Turn "initialized" flag off. */
	cmgdm.lginit = FALSE;

	return;
}

