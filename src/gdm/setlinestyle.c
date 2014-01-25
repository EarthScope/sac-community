
#include <stdio.h>
#include "gdm.h"

/** 
 * Change the linestyle
 *
 * @param istyle
 *    Linestyle attribute to set
 *    - 1 for a solid line
 *    - > 1 for a device specific line style
 *
 * @date   841108:  Original version.
 *
 */
void 
setlinestyle(int istyle)
{

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();
	/* - If linestyle is different than current one: */
	if( istyle > 0 && istyle != cmgdm.iline ){

		/* -- Save new linestyle. */
		cmgdm.iline = istyle;

		/* -- Send new linestyle request to all active graphics devices. */
                for(i = 0; i < n; i++) {
                  if(dev[i]->on && dev[i]->set_line_style) {
                    dev[i]->set_line_style( &cmgdm.iline );
                  }
                }
        }
}

