
#include "co.h"
#include "gdm.h"

/** 
 * Set the current color attribute
 *
 * @param number 
 *    Number of the desired color to set
 *    - 1 for normal device color
 *    - > for a device specific color
 *
 * @date   900911:  Bug fix: setcolorN was only being called if a new color
 *                  was being set. Doesn't allow for changing devices. (bkh)
 * @date   831026:  Original version.
 *
 */
void 
setcolor(int number)
{
	int ncolor;

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	/* - Range check the requested color number. */
	ncolor = min( cmgdm.nctsize, max( 0, number ) );
	/*      if(ncolor.eq.0)ncolor=nctsize */

	/* - Save the current color without regard to the device being used. */
	cmgdm.icolor = ncolor;

	/* -- Set color for all active graphics devices. */
        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->set_color) {
            dev[i]->set_color( ncolor );
          }
        }

}

