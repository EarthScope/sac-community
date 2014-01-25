/** 
 * @file drawpoly3.d
 *
 */

#include <stdlib.h>
#include "gd3.x11.h"

#include "config.h"

/** 
 * To draw npts poly-segment lines through npts points.
 * 
 * @param xloc_vp
 *    X location of points to draw in viewport coordinates
 * @param yloc_vp
 *    Y location of points to draw in viewport coordinates
 * @param npts
 *    Number of points to draw
 *
 * @date   920317:  Changed size of ptlist from 100 to 102. - wct.
 * @date   910212:  Original Version  (jjy)
 */

void 
drawpoly3(float *xloc_vp,
          float *yloc_vp,
          int npts)
{
  static int i;
  static XPoint ptlist[102];
  XWindow *xw;
  xw = plot_window( CURRENT );

  /* w = xw->width - 1; */
  /* h = xw->height - 1; */

/* Convert points from viewport coords to pixels */
  for (i = 0; i<npts; i++) {
    ptlist[i].x = view_to_x11_x( xloc_vp[i], xw );
    ptlist[i].y = view_to_x11_y( yloc_vp[i], xw );
  }

/* Draw line */

  XSetForeground(DISPLAY(xw), xw->gc,color3);

#ifndef USE_X11_DOUBLE_BUFFER
  XDrawLines(DISPLAY(xw), xw->win, xw->gc, ptlist, npts, CoordModeOrigin);

#else /* USE_X11_DOUBLE_BUFFER */
  XDrawLines(DISPLAY(xw), xw->buffer, xw->gc, ptlist, npts, CoordModeOrigin);
#endif /* USE_X11_DOUBLE_BUFFER */

}


void
fillpoly3(float *x, float *y, int n) {
  int i;
  XWindow *xw;
  XPoint *p;
  xw = plot_window(CURRENT);

  if(n <= 0) {
    return;
  }
  p = (XPoint *) malloc(sizeof(XPoint) * n);
  for(i = 0; i < n; i++) {
    p[i].x = view_to_x11_x(x[i], xw);
    p[i].y = view_to_x11_y(y[i], xw);
  }

  XSetForeground(DISPLAY(xw), xw->gc,color3);  
  XFillPolygon(DISPLAY(xw), xw->buffer, xw->gc, p, n, Complex, CoordModeOrigin);

  if(p) {
    free(p);
    p = NULL;
  }
}
