/** 
 * @file move3.c
 *
 */

#include "gd3.x11.h"

/** 
 * Move to a Viewport Location
 *
 * @param xloc_vp
 *   X Location to move to in Viewport coordinate
 * @param yloc_yp
 *   Y Location to move to in Viewport coordinate
 *
 * @date   890608:  Used as-is from X10 version.  (kjm)
 * @date   870318:  Changes due to gd3.x10.h structure change.
 * @date   870223:  Original Version
 *
 */
void 
move3(float xloc_vp,
      float yloc_vp)
{
  XWindow *xw;

  xw = plot_window( CURRENT );

  draw_pos.x = view_to_x11_x(xloc_vp, xw);
  draw_pos.y = view_to_x11_y(yloc_vp, xw);

}

/* All values are scaled by the width 
 *   Normally scaling would be done by the width and height 
 *   But, the drawing function use the aspect ratio to scale the line lengths and positions
 *    properly
 */

float
view_to_x11_x(float x, XWindow *xw) {
  return x * xw->width;
}
float
view_to_x11_y(float y, XWindow *xw) {
  //  return xw->height - (y * xw->height);
  return xw->height - (y * xw->width);
}

float
x11_to_view_x(float x, XWindow *xw) {
  return x / (float) xw->width;
}
float
x11_to_view_y(float y, XWindow *xw) {
  //  return (xw->height - y) / (float)xw->height;
  return (xw->height - y) / (float)xw->width;
}
