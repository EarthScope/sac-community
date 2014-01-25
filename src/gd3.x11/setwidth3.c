/** 
 * @file setwidth3.c
 *
 */

#include "gd3.x11.h"

/** 
 * To set the graphics line-width.
 *
 * @param index
 *    Width of line in pixels
 *
 * @date   920526:  Original version.
 *
 */
void 
setwidth3(int index) {

  XGCValues gcv;
  XWindow *xw;

  xw = plot_window( CURRENT );

  /* Set line-width */
  gcv.line_width = index;
  XChangeGC(DISPLAY(xw), xw->gc, GCLineWidth, &gcv);
}
