

#include "config.h"
#include "debug.h"
#include "gd3.x11.h"

void
expose3() {
  
#ifdef USE_X11_DOUBLE_BUFFER
  XExposeEvent event;
  XWindowAttributes attributes;
  XWindow *xw;

  xw = plot_window( CURRENT );
  
  if(!xw) {
      return;
  }
  
  XGetWindowAttributes(DISPLAY(xw), xw->win, &attributes);
  
  event.type       = Expose;
  event.display    = DISPLAY(xw);
  event.window     = xw->win;
  //event.send_event = True;
  event.x          = 0;
  event.y          = 0;
  event.width      = attributes.width;
  event.height     = attributes.height;
  event.count      = 0;
  
  DEBUG("size: [%d %d] id: %d\n", attributes.width, attributes.height, xw->win);
  DEBUG("queue: %d\n", XPending( DISPLAY(xw) ));
  XSendEvent(DISPLAY(xw), xw->win, False, ExposureMask, (XEvent *) &event);
#endif /* USE_X11_DOUBLE_BUFFER */

}

