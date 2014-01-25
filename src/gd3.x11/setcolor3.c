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
setcolor3(int index)
{
  int index2;
  XScreen *xs;
  XWindow *xw;

  xw = plot_window(CURRENT);
  xs = xscreen_get();

  if (XDisplayCells(xs->display, xs->screen) > index){
    index2 = index;
    if(cmgam.cmap != MDEFAULT) index2 += 7; /* if MGREY or MCOLOR skip over */
                                            /* the first seven entries. */
    color3 = pixdef3[index2].pixel;
    xw->color = &( pixdef3[index2] );
    DEBUG("setcolor3: %ld %ld [default]\n", color3, xw->color->pixel);
  }
  else {
    color3 = BlackPixel(xs->display, xs->screen);
    xw->color = &( pixdef3[cmgdm.nctsize] );
    DEBUG("setcolor3: %ld %ld [default]\n", color3, xw->color->pixel);
  }
}

