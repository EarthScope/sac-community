
#include "gdm.h"
#include "gem.h"

/** 
 * Set the line width
 *
 * @param nwidth 
 *    Width of the desired line
 *    - 1 is a normal width
 *
 * @date   920527:  Original version
 *
 */
void 
setlinewidth(int nwidth)
{

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	/* - The reason for this next test is.... that the sgftops conversion
	 *   utility checks for a width option, passwd by the prn script,
	 *   which makes all the lines a specified width, and so the 
	 *   WIDTH command will override that width with other setlinewidth
	 *   commands throughout the Postscript file. */
	if( !cmgem.lwidth )
		goto L_8888;

	/* -- Set line width for all active graphics devices. */
        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->set_line_width) {
            dev[i]->set_line_width( nwidth );
          }
        }

L_8888:
	return;
}

