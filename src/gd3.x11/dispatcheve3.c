/*******************************************************************************
** PURPOSE:
*    To handle window events.  Processes all window events in queue.
*
** OUTPUT ARGUMENTS:
*    nerr:  Error flag.  Set to 0 if no error occurs. (Pointer)
*
** GLOBAL INPUT:
*    plotw3->(win, width_p, height_p), basew3->(win, width_p, height_p),
*    titlew3->(win, height_p), iconw3->win, num_wins3
*    title_font3, cursor_on3
*
** GLOBAL OUTPUT:
*    plotw3->(width_p, height_p), titlew3->width, basew3->(width_p, height_p),
*    cursor_on3, xcursor_p3, ycursor_p3, char_cursor3,
*
** SUBROUTINES CALLED:
*    XPending, XGetWindowAttributes, XNextEvent, XMoveResizeWindow,
*    XDrawString, XQueryPointer
*    XStoreBitmap, XBitmapBitsPut, XFreeBitmap, XLookupString,
*    XDrawLine, XFlush, make_label3, strncpy
*
** LOCAL VARIABLES:
*    pevent:        Pointer to event. (Pointer)
*    event:         Event.
*    index:         Window number which event occurred in.
*    i, m, n:       Loop counters.
*    nevents:       Number of events in queue to process.
*    nbytes:        Equals 0 of no character was struck at cursor.
*    is_basew:      Set to 1 if event is in base window.
*    is_titlew:     Set to 1 if event is in title window.
*    is_plotw:      Set to 1 if event is in plot window.
*    is_iconw:      Set to 1 if event is in icon window.
*    done_text:     Set to 1 as soon as a <cr> is struck.
*    first_char:    Set to 1 if reading first character of text.
*    title_label:   Title label.
*    ptitle_label:  Pointer to title_label. (Pointer)
*    kchar:         Character string struck at cursor. (Pointer)
*******************************************************************************
** MODIFICATION HISTORY:
*    910506:  Added kludge to fix cursor location problem in OpenWindows
*             versions 2 and 3. Not sure when Sun will fix the problem.
*    890607:  Modified to run under X11 rather than X10.    (kjm)
*    870323:  Added handling of line of cursor text events.
*    870318:  Changes due to gd3.x10.h structure change.
*    870310:  Added icon.
*    870309:  Added title window.
*    870302:  Added handling of resize events.
*    870227:  Original Version
*******************************************************************************/

#include <string.h>
#include <stdlib.h>

#include "string_utils.h"

#include "gd3.x11.h"

#include "bool.h"
#include "proto.h"
#include "exm.h"
#include "select.h"
#include "config.h"

#define MOTION   2

void x11_replay();

char *
event_type(XEvent *e) {
  switch(e->type) {
  case KeyPress:         return "KeyPress";  break;
  case KeyRelease:       return "KeyRelease";  break;    
  case ButtonPress:      return "ButtonPress";  break;
  case ButtonRelease:    return "ButtonRelease";  break;
  case MotionNotify:     return "MotionNotify";  break;
  case EnterNotify:      return "EnterNotify";  break;
  case LeaveNotify:      return "LeaveNotify";  break;
  case FocusIn:          return "FocusIn";  break;
  case FocusOut:         return "FocusOut";  break;
  case KeymapNotify:     return "KeymapNotify";  break;
  case Expose:           return "Expose";  break;
  case GraphicsExpose:   return "GraphicsExpose";  break;
  case NoExpose:         return "NoExpose";  break;
  case VisibilityNotify: return "VisibilityNotify";  break;
  case CreateNotify:     return "CreateNotify";  break;
  case DestroyNotify:    return "DestroyNotify";  break;
  case UnmapNotify:      return "UnmapNotify";  break;
  case MapNotify:        return "MapNotify";  break;
  case MapRequest:       return "MapRequest";  break;
  case ReparentNotify:   return "ReparentNotify";  break;
  case ConfigureNotify:  return "ConfigureNotify";  break;
  case ConfigureRequest: return "ConfigureRequest";  break;
  case GravityNotify:    return "GravityNotify";  break;
  case ResizeRequest:    return "ResizeRequest";  break;
  case CirculateNotify:  return "CirculateNotify";  break;
  case CirculateRequest: return "CirculateRequest";  break;
  case PropertyNotify:   return "PropertyNotify";  break;
  case SelectionClear:   return "SelectionClear";  break;
  case SelectionRequest: return "SelectionRequest";  break;
  case SelectionNotify:  return "SelectionNotify";  break;
  case ColormapNotify:   return "ColormapNotify";  break;
  case ClientMessage:    return "ClientMessage";  break;
  case MappingNotify:    return "MappingNotify";  break;
    //case GenericEvent:     return "GenericEvent";  break;
  default: return "Unknown Event"; break;
  }
  return "Unknown Event";
}

int
use_large_crosshairs(int getset) {
  static int virgin = TRUE;
  static int use_xhairs = FALSE;
  if(getset == OPTION_ON || getset == OPTION_OFF) {
    use_xhairs = getset;
  }
  if(virgin) {
    virgin = FALSE;
    use_xhairs = env_bool(SAC_PPK_LARGE_CROSSHAIRS, use_xhairs);
  }
  return use_xhairs;
}


void
cursor_char(XEvent *e) {
  int nbytes;
  char kchar;
  XKeyEvent *key;
  /* Get cursor position and if applicable, the character struck */
  /* Also, set cursor_on3 flag to indicate cursor is off */
  key = (XKeyEvent *) e;
  xcursor_p3 = key->x;
  ycursor_p3 = key->y;
  
  nbytes = XLookupString(key, &kchar,1,NULL,NULL);
  if (nbytes > 0) {
    char_cursor3[0] = kchar;
  }
  cursor_on3 = 0;
}

void
cursor_text_string(XWindow *plot, XEvent *e) {
  int m, done_text, first_char;
  int nbytes;
  char kchar;
  XKeyEvent *key;
  /* Get cursor position and a line of text */
  m = 0;
  done_text = 0;
  first_char = 1;
  while (!done_text) {
    key = (XKeyEvent *) e;
    nbytes = XLookupString(key,&kchar,1,NULL,NULL);
    if (nbytes > 0) {
      if (first_char) {  /* Save position of first character */
        xcursor_p3 = key->x;
        ycursor_p3 = key->y;
        first_char = 0;
      }
      if (kchar == 13) {
        done_text = 1;
        text_cursor3[m] = '\0';
        cursortext_on3 = 0;
      } else {
        text_cursor3[m] = kchar;
        m++;
        XNextEvent(DISPLAY(plot), e);
      }
    } else {
      XNextEvent(DISPLAY(plot), e);
    }
  }
}

void
xwindow_compress_event(XWindow *xw, XEvent *event) {
  while(XCheckTypedWindowEvent(DISPLAY(xw), xw->win, event->type, event) == True) {  }
}

int
xwindow_blocking() {
  if(cursor_on3 || cursortext_on3) {
    return TRUE;
  }
  return FALSE;
}

int
xwindow_get_event(XScreen *xs, XEvent *e) {
  if(xwindow_blocking()) {
    XNextEvent(xs->display, e);
    return TRUE;
  }
  if(XPending(xs->display) > 0) {
    XNextEvent(xs->display, e);
    return TRUE;
  }
  return FALSE;
}

void 
crosshairs_draw(XWindow *xw, XEvent *pe) {
  XButtonEvent *e = (XButtonEvent *) pe;
  XDrawLine(DISPLAY(xw), xw->win, xw->gc, 0, e->y, xw->width-1, e->y);
  XDrawLine(DISPLAY(xw), xw->win, xw->gc, e->x, 0, e->x, xw->height-1);
  
}

void
dispatchevent3(int *nerr) {
  XEvent event, *pevent = &event;
  int index, i;
  int is_plotw = 0;

  XScreen *xs;
  XWindow *plot, *xw;

  static int use_massive_crosshairs = -1;
  
  static int button_state = FALSE;
  static int start_x      = 0;
  static int start_y      = 0;

  if(use_massive_crosshairs == -1) {
    use_massive_crosshairs = use_large_crosshairs(OPTION_GET);
  }

  *nerr = 0;

/* Find out which window event occurred in and set index */

  xs = xscreen_get();
  while(xwindow_get_event(xs, &event)) {
      xw = NULL;
      index = 0;
      i = 1;
      do {
        if (((XAnyEvent *) pevent)->window == plotw3[i].win) {
          index = i;
          is_plotw = 1;
          xw = &(plotw3[index]);
          break;          /* Plot window */
        }
        else if (((XAnyEvent *) pevent)->window == basew3[i].win) {
            index = i;
            xw = &(basew3[index]);
            break;          /* Base window */
        }
        else if (((XAnyEvent *) pevent)->window == titlew3[i].win) {
            index = i;
            xw = &(titlew3[index]);
            break;          /* Title window */
        }
        else
            i++;
      }
      while (i <= num_wins3);

      plot  = &(plotw3[index]);

      if(!xw || xw->win == 0) {
          continue;
      }
/*  Process each event */

      switch (event.type) {

#ifdef USE_X11_DOUBLE_BUFFER
      case DestroyNotify:
	break;
      case UnmapNotify:
	break;
#endif

      /* George Helffrich <george@geology.bristol.ac.uk>
         If you can, handle window destroy events triggered by window manager
	 here. These are clicks on the close button on the window frame. I
	 choose to ignore them and leave the window active -- SAC has no
	 semantics for closing windows except enddevice.

      case ClientMessage:
        *nerr = 1;
        break;
      */

      case ConfigureNotify:
        {
          XConfigureEvent *e = (XConfigureEvent *) pevent;
          xwindow_compress_event(xw, pevent);
          xw->resize(xw, e->width, e->height, xw->resize_data);
          if(is_plotw) {
            expose3();
          }
	}
	break;

      case Expose:
        {
          XExposeEvent *e = (XExposeEvent *) pevent;
          xwindow_compress_event(xw, pevent);
          if(e->count == 0) {
            xw->expose( xw, xw->expose_data );
          }
        }
	break;

      case KeyPress:
        {
	  if ((cursor_on3) && (is_plotw)) {
            cursor_char( pevent );
	  } else if ((cursortext_on3) && (is_plotw)) {
            cursor_text_string( plot, pevent );
	  } 
        }
        break;

      case ButtonPress:
        {
          XButtonEvent *e = (XButtonEvent *) pevent;
          if( is_plotw ) {
            button_state = TRUE;
            start_x = e->x;
            start_y = e->y;
            if( cursortext_on3 ) {
              cursor_text_string( plot, pevent );
            } 
          }
        }
        break;
      case ButtonRelease:
        {
          XButtonEvent *e = (XButtonEvent *) pevent;
          if(is_plotw) {
            xwindow_expose(plot, plot->expose_data);
            crosshairs_draw(plot, pevent);
            if((start_x != e->x || start_y != e->y) && button_state == MOTION) {
              /* End Button Window Press */
              char_cursor3[0] = 'x';
              xcursor_p3 = e->x;
              ycursor_p3 = e->y;
              cursor_on3 = FALSE;
            } else if(button_state == TRUE && 
                      start_x == e->x && start_y == e->y) {
              char_cursor3[0] = 'L';
              xcursor_p3 = e->x;
              ycursor_p3 = e->y;
              cursor_on3 = FALSE;
            }
            button_state = FALSE;
          }
        }
        break;

      case LeaveNotify:
      case EnterNotify:
      case MotionNotify:
          {
            XMotionEvent *e = (XMotionEvent *) pevent;

            if(button_state || 
               (( cursor_on3 || cursortext_on3) && use_massive_crosshairs)) {
              xwindow_expose(plot, plot->expose_data);
            }              
            if(button_state) {
              GC igc;
              XGCValues val;
              val.function = GXinvert;

              /* Invert Color for Zoom */
              igc = XCreateGC(DISPLAY(plot), plot->win, GCFunction, &val);
              if(start_x <= e->x) {
                XFillRectangle(DISPLAY(plot), xw->win, igc, 
                               start_x, 0, e->x - start_x, xw->height);
              } else {
                XFillRectangle(DISPLAY(plot), xw->win, igc, 
                               e->x, 0, start_x - e->x, xw->height);
              }
              if(button_state == TRUE) {
                xcursor_p3 = start_x;
                ycursor_p3 = start_y;
                char_cursor3[0] = 'x';
                cursor_on3 = FALSE;
                button_state = MOTION;
              }
            }
            if ((cursor_on3 || cursortext_on3) && (is_plotw)) {
              if(use_massive_crosshairs) {
                /* Massive Cross Hairs */
                crosshairs_draw(plot, pevent);
              }
            }
          }

          break;
      }

      /* Reset window flags */

      is_plotw = 0;

      }
}

int
get_file_descriptor_x11( void ) {
  XScreen *xs;
  xs = xscreen_get();
  if(xs) {
    return ConnectionNumber( xs->display );
  }
  return -1;
}
