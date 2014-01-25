
#include <string.h>

#include "gdm.h"

/** 
 * Tell graphics library that the next plots will be using the cursor. 
 *  
 * This routine temporarily turns off all graphics devices, except for the
 *   one enabled for graphics input 
 *
 * @date   831027:  Original version.
 *
 */
void 
cursoron(void)
{
	int nerr;

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	/* - For each graphics device:
	 *   (1) If it is on and not the cursor device, turn it off.
	 *   (2) If it is off and the cursor device, turn in on.
	 *   (3) Otherwise, do nothing. */
	/* - Set a flags so that cursoroff will know how to undo what cursoron did. */
        for(i = 0; i < n; i++) {
             if(dev[i]->id == REC) {
                continue;
            }
          if(dev[i]->on && ! dev[i]->cursor_enabled) {
            enddevice( dev[i]->name, strlen(dev[i]->name), &nerr);
            dev[i]->cursor_flag = -1;
          } else if( ! dev[i]->on && dev[i]->cursor_enabled) {
            begindevice( dev[i]->name, strlen(dev[i]->name), &nerr);
            dev[i]->cursor_flag = 1;
          } else {
            dev[i]->cursor_flag = 0;
          }
        }
}

