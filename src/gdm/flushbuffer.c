
#include "gdm.h"

/** 
 * Flush the buffers of all graphics devices
 *
 * @param nerr
 *   Error Return flag
 *   - 0 on Success
 *   - Non-Zero on Error
 *    
 * @date   861014:  Original version.
 *
 */
void 
flushbuffer(int *nerr)
{

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	*nerr = 0;

	/* - Flush the buffers for each active graphics device. */
        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->flush_buffer) {
            dev[i]->flush_buffer( nerr );
          }
        }

	return;

}

