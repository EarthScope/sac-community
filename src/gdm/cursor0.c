/** 
 * @file   cursor0.c
 * 
 * @brief  Cursor Function
 * 
 */

#include "gdm.h"

/** 
 * To perform "locator" graphics input function.
 *       The cursor is turned on and initially placed at xloc
 *       and yloc.  When a single character is typed at the
 *       terminal, the new cursor location and character are returned.
 * 
 * @param xloc 
 *    Input:  Initial X plot coordinate for cursor. [f]
 *    Output: Current X plot coordinate for cursor. [f]
 * @param yloc 
 *    Input:  Initial Y plot coordinate for cursor. [f]
 *    Output: Current Y plot coordinate for cursor. [f]
 * @param kchar 
 *    Alphanumeric character typed at terminal. [c1]
 *
 * @date   831026:  Original version.
 *
 */
void 
cursor0(float *xloc, 
        float *yloc, 
        char  *kchar) {

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

        for(i = 0; i < n; i++) {
          if(dev[i]->on) {
            if(dev[i]->move && dev[i]->cursor) {
              dev[i]->move(*xloc, *yloc);
              dev[i]->cursor(xloc, yloc, kchar, 1);
            }
          }
        }
}

