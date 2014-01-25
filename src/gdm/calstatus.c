/** 
 * @file calstatus.
 * @brief Compute Graphic Device status variables  
 */

#include "gdm.h"
#include "bool.h"

/** 
 * Compute several graphic device status variables
 *
 * @date   861020:  Original version.
 *
 */
void
calstatus() {

	/* - For each graphics device that is on:
	 * -- If it is a passive device set lpasiv to .TRUE.
	 * -- If it is an active device set lactiv to .TRUE. */
	cmgdm.lactiv = FALSE;
	cmgdm.lpasiv = FALSE;

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

        for(i = 0; i < n; i++) {
          if(dev[i]->on && ! dev[i]->mirror_device ) {
            cmgdm.lactiv =   dev[i]->active_device;
            cmgdm.lpasiv = ! dev[i]->active_device;
          }
        }
        
	/* - See if there is a graphics device that is on that can perform
	 *   graphics input (cursor) functions. */

	cmgdm.lcur = FALSE;
	cmgdm.igdcur = -1;

        for(i = 0; i < n; i++) {
          if(dev[i]->on) {
            if(dev[i]->cursor_enabled == TRUE) {
              cmgdm.lcur = TRUE;
              cmgdm.igdcur = dev[i]->id;
            }
          }
        }

	return;


}

