/** 
 * @file draw3.c
 *
 */

#include "gd3.x11.h"

#include "config.h"
/** 
 * To draw to a given viewport location. 
 *
 * @param xloc_vp
 *    X location in viewport coordinates to draw to. 
 * @param yloc_vp
 *    Y location in viewport coordinates to draw to. 
 *
 * @date   890608:  Modified to run under X11 rather than X10.  (kjm)
 * @date   870318:  Changes due to gd3.x10.h structure change.
 * @date   870223:  Original Version
 *
 */
void 
draw3(float xloc_vp,
      float yloc_vp)
{
  point new_pt_p;
  XWindow *xw;

  xw = plot_window( CURRENT );

  /* w = xw->width - 1; */
  /* h = xw->height - 1; */

/* Convert point from viewport coords to pixels */

  new_pt_p.x = view_to_x11_x(xloc_vp, xw);
  new_pt_p.y = view_to_x11_y(yloc_vp, xw);

/* Draw line */

  XSetForeground(DISPLAY(xw), xw->gc,color3);

#ifndef USE_X11_DOUBLE_BUFFER
  XDrawLine(DISPLAY(xw), xw->win, xw->gc, 
	    draw_pos.x, draw_pos.y,
	    new_pt_p.x, new_pt_p.y);
#else /* USE_X11_DOUBLE_BUFFER */
  XDrawLine(DISPLAY(xw), xw->buffer, xw->gc, 
	    draw_pos.x, draw_pos.y,
	    new_pt_p.x, new_pt_p.y);
#endif /* USE_X11_DOUBLE_BUFFER */

  /* Update current point */
  draw_pos.x = new_pt_p.x;
  draw_pos.y = new_pt_p.y;

}
