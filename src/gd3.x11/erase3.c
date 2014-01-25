/*******************************************************************************
** PURPOSE:
*    To erase the window.
*
** GLOBAL INPUT:
*    gd3.x11.h:  plotw3->win
*
** SUBROUTINES CALLED:
*    XClearWindow, XFlush
*******************************************************************************/

#include <stdio.h>

#include "gd3.x11.h"

#include "config.h"

#include "gem.h"
#include "gdm.h"


void
fill_background3(int window) {
  int color;

  XWindow *w = plot_window( window );

  color = (color_on()) ? color_background() : color_background_default() ;
  setcolor(color);

  XSetForeground(DISPLAY(w), w->gc, color3);
  XSetBackground(DISPLAY(w), w->gc, color3);

  XFillRectangle(DISPLAY(w), w->buffer, w->gc, 0, 0, w->width, w->height);

  color = (color_on()) ? color_foreground() : color_foreground_default() ;
  setcolor(color);

}

void
erase3() {

/* Erase window */

#ifdef USE_X11_DOUBLE_BUFFER
  fill_background3(c_win3);
#endif /* USE_X11_DOUBLE_BUFFER */

}

/*******************************************************************************
** MODIFICATION HISTORY:
*    890608:  Modified to run under X11 rather than X10.  (kjm)
*    870318:  Changes due to gd3.x10.h structure change.
*    870223:  Original Version
*******************************************************************************/
