/** 
 * @file changectable
 *
 * @brief Change the Color Table
 *
 */
#include "gdm.h"

/** 
 * Change SAC's auxilliary color 
 *
 * @param nentry 
 *   Number of entries in the standard color table
 * @param ctable
 *   Requested color table
 *
 * @date   940921:  Original version.
 *
 */
void 
changectable(int nentry,
             int ctable) {

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	/* - Send color table to all active graphics devices. */
        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->change_color_table) {
            dev[i]->change_color_table(nentry, ctable);
          }
        }


}

