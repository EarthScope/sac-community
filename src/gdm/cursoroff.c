
#include <string.h>

#include "gdm.h"

/** 
 * Reverse what cursoron did
 *
 * @date   831027:  Original version.
 */
void 
cursoroff(void)
{
	int nerr;

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	/* - For each graphics device:
	 *   (1) Turn in back on, if it was temporarily turned off.
	 *   (2) Turn in off, if it was temporarily turned on.
	 *   (3) Reset flags to zero. */

        for(i = 0; i < n; i++) {
            if(dev[i]->id == REC) {
                continue;
            }
          if(dev[i]->cursor_flag < 0) {
            begindevice(dev[i]->name, strlen(dev[i]->name), &nerr);
          }
          if(dev[i]->cursor_flag > 0) {
            enddevice(dev[i]->name, strlen(dev[i]->name), &nerr);
          }
          dev[i]->cursor_flag = 0;
        }
}

