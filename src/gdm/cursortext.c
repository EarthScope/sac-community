
#include "gdm.h"

/** 
 * Get Cursor location and text from the "Graphics Window".
 *
 * The cursor is turned on and initially placed at \p xloc and \p yloc.
 *   When a line of text terminated by a carriage return is typed in the
 *   graphics window, the new cursor location and text are returned.
 *
 * Only the first active device is used.
 *
 * @param xloc
 *   Initial x location on input 
 *   Current x location on output
 * @param yloc
 *   Initial y location on input 
 *   Current y location on output
 * @param ktext
 *   Text typed in the graphics window
 * @param ktext_s
 *   Length of text typed on input
 *
 * @date   870323:  Original version based upon cursor.
 *
 */
void 
cursortext(float *xloc,
           float *yloc,
           char *ktext,
           int ktext_s)
{
  
        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

        for(i = 0; i < n; i++) {
          if(dev[i]->on) {
            if(dev[i]->move && dev[i]->cursor_text) {
              dev[i]->move(*xloc, *yloc);
              dev[i]->cursor_text(xloc, yloc, ktext, ktext_s);
              break;
            }
          }
        }

}

