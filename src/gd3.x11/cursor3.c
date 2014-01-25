/*******************************************************************************
** PURPOSE:
*    To return the location of the cursor and the character struck.
*
** INPUT:
*
*    cchar:  Character returned if no character was struck (mouse button
*           was pressed).
*
** OUTPUT ARGUMENTS:
*    xloc_vp:       x location of cursor in viewport coordinates. (Pointer)
*    yloc_vp:       y location of cursor in viewport coordinates. (Pointer)
*    cchar:         Character struck.  If no character was struck (mouse
*                   button was pressed) the value of cchar at input time will
*                   be returned.
*    cchar_length:  Length of cchar. (Pointer)
*
** GLOBAL INPUT:
*    gd3.x11.h:  plotw3->(win, width_p, height_p), c_win3, xcursor_p3, ycursor_p3,
*                cursor_on3, char_cursor3
*
** GLOBAL OUTPUT:
*    gd3.x11.h:  cursor_on3, char_cursor3
*
** SUBROUTINES CALLED:
*    XQueryPointer, XDrawLine, XFlush, XBell, XSelectInput, dispatchevent3_,
*
** LOCAL VARIABLES:
*    subw:          Subwindow, if any, which mouse is in.
*    mouse_status:  Status of mouse.
*    nerr:          Error flag.
*    mouse_x_p:     X position of mouse in pixels.
*    mouse_y_p:     Y position of mouse in pixels.
*    pvendor:       Pointer to vendor string of display structure.
*    owoffset:      Set to zeor if MIT X11R4, otherwise set to the offset required
*                   to fix the cursor hotspot problem in OpenWindows
*******************************************************************************
** MODIFICATION HISTORY:
*    920609:  Changed hardcoded valie for OW_OFFSET in ../../inc/gd3.x11.h .
*             XDrawLine call-back problem fixed in OpenWindows version 3.
*    910506:  Added kludge to fix cursor location problem in OpenWindows
*             versions 2 and 3. Not sure when Sun will fix the problem.
*    890608:  Modified to run under X11 rather than X10.  (kjm)
*    880208:  Modified so that cchar is an input and an output.
*    870318:  Changes due to gd3.x11.h structure change.
*    870303:  Original Version
*******************************************************************************/

#include "gd3.x11.h"
#include "bool.h"
#include "debug.h"

extern int bellON;

void 
cursor3(float *xloc_vp,
	float *yloc_vp,
	char cchar[],
	int cchar_length)
{
  int nerr;

/* Use an offset of OW_OFFSET for calls to XDrawLine. OpenWindows has a problem
   that does not occur in generic X11R4. */

  XWindow *xw;
  UNUSED(cchar_length);
  xw = plot_window( CURRENT );

  if(bellON) {
    XBell(DISPLAY(xw),25); 
  }
    
/* Select which events the plot window needs to accept for cursor input */

  XSelectInput(DISPLAY(xw),xw->win,
	       KeyPressMask          |
	       ButtonPressMask       |
	       ButtonReleaseMask     |
	       EnterWindowMask       |
	       LeaveWindowMask       |
	       PointerMotionMask     |
	       StructureNotifyMask   |
	       ExposureMask);

/* Initialize character struck */

  char_cursor3[0] = cchar[0];

/* While waiting for cursor event, handle other events */

  expose3();

  cursor_on3 = TRUE;

  dispatchevent3(&nerr);

  /* Don't accept events anymore for cursor input */

  XSelectInput(DISPLAY(xw),xw->win,
	       (StructureNotifyMask | ExposureMask));

/* Set location of cursor and character struck */

  *xloc_vp = x11_to_view_x(xcursor_p3, xw);
  *yloc_vp = x11_to_view_y(ycursor_p3, xw);

  cchar[0] = char_cursor3[0];

}

