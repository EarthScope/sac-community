/** 
 * @file setcolor3.c
 *
 */

#include "gd3.x11.h"
#include "gam.h"
#include "debug.h"

/** 
 * Set the Graphics Color
 *
 * @param color
 *    Index of the color to set in the color table 
 *
 * @date   890608:  Modified to run under X11 rather than X10.  (kjm)
 * @date   870316:  Modification due to change of arrangement of color table.
 * @date   870310:  Original Version
 */
void
setcolor3(color c) {
    XScreen *xs;
    XWindow *xw;

    xw = plot_window(CURRENT);
    xs = xscreen_get();

    

    if(xw->color.red   != c.r * 256 ||
       xw->color.green != c.g * 256 ||
       xw->color.blue  != c.b * 256) {
        xw->color.red   = c.r * 256;
        xw->color.green = c.g * 256;
        xw->color.blue  = c.b * 256;
        xw->color.flags = DoRed | DoGreen | DoBlue;
        XAllocColor(xs->display, DefaultColormap(xs->display, xs->screen), &xw->color);
    }
    return;
}
