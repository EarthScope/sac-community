
#include <stdlib.h>
#include <string.h>

#include "co.h"
#include "gdm.h"
#include "gem.h"
#include "bool.h"

GEM_EXTERN

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
setcolor(color c) {

    int i, n;
    display_t **dev;
    n = gdm_get_ndevices();
    dev = gdm_get_devices();

    /* -- Set color for all active graphics devices. */
    for (i = 0; i < n; i++) {
        if (dev[i]->on && dev[i]->set_color) {
            dev[i]->set_color(c);
        }
    }

}
extern color COLORS[];

int
color_index_to_rgb(int index, color *c) {
    if(index < 0 || index > 7) {
        return FALSE;
    }
    *c = COLORS[index];
    return TRUE;
}

void
setcolor_index(int index) {
    color c;
    if(color_index_to_rgb(index, &c)) {
        setcolor(c);
    }
}

extern color COLOR_FG_DEFAULT;
extern color COLOR_BG_DEFAULT;

void setcolor_fg()     { setcolor(cmgem.icol); }
void setcolor_bg()     { setcolor(cmgem.ibacol); }
void setcolor_skel()   { setcolor(cmgem.iskcol); }
void setcolor_fg_def() { setcolor( COLOR_FG_DEFAULT ); }
void setcolor_bg_def() { setcolor( COLOR_BG_DEFAULT ); }
