
#include "co.h"
#include "gdm.h"

/** 
 * Get information about the alphanumeric characteristics of the active
 *   graphics device.
 *
 * This routine get the number of lines in the current terminal window
 *
 * @param nlines
 *   Number of text lines per screen
 *   Default output is 23 lines
 * @param erase
 *   Text to send to erase the terminal screen
 *   Set to all blanks if terminal can scroll
 * @param erase_s
 *   Length of \p erase
 *
 * @date   870217:  Changed name from getscreenatr.
 * @date   861020:  Original version.
 *
 */
void 
getalphainfo(int *nlines, char *erase, int erase_s)
{

        int nerr;
        int ncols;
        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	/* - Inquire about text values from appropriate graphics devies. */

        *nlines = -1;
        fstrncpy( erase, erase_s-1, " ", 1 );
        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->get_alpha_info) {
            dev[i]->get_alpha_info(nlines, erase, erase_s);
          }
        }

        if(*nlines == -1) {
          zgwindowsize_(nlines, &ncols, &nerr);
          if(nerr) {
            *nlines = 23;
          }
        }
        return;

}

