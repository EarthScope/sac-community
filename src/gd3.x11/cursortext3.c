/*******************************************************************************
** PURPOSE:
*    To return the location of the cursor and the line of text input.
*
** OUTPUT ARGUMENTS:
*    xloc_vp:       x location of cursor in viewport coordinates. (Pointer)
*    yloc_vp:       y location of cursor in viewport coordinates. (Pointer)
*    ktext:         Line of text input.
*    ktext_length:  Length of cchar. (Pointer)
*
** GLOBAL INPUT:
*    gd3.x11.h:  plotw3->(win, width_p, height_p), c_win3, xcursor_p3, ycursor_p3,
*                cursortext_on3, text_cursor3
*
** GLOBAL OUTPUT:
*    gd3.x11.h:  cursortext_on3
*
** SUBROUTINES CALLED:
*    XQueryPointer, XDrawLine, XFlush, XBell, XSelectInput, dispatchevent3_
*
** LOCAL VARIABLES:
*    child:         Child window, if any, which mouse is in.
*    root:          Root window of window mouse is in.
*    rootx,y:       Mouse location relative to root window origin.
*    mask:          Mask_value from XQueryPointer.
*    mouse_status:  Status of mouse.
*    nerr:          Error flag.
*    mouse_x_p:     X position of mouse in pixels.
*    mouse_y_p:     Y position of mouse in pixels.
*******************************************************************************
** MODIFICATION HISTORY:
*    910506:  Added kludge to fix cursor location problem in OpenWindows
*             versions 2 and 3. Not sure when Sun will fix the problem.
*    890608:  Modified to run under X11 rather than X10.  (kjm)
*    870318:  Changes due to gd3.x10.h structure change.
*    870303:  Original Version
*******************************************************************************/

#include <string.h>

#include "gd3.x11.h"
#include "bool.h"
#include "debug.h"

extern int bellON;

void 
cursortext3(float *xloc_vp,
            float *yloc_vp,
            char ktext[],
            int ktext_length)
{
  int nerr;
/* Use an offset of OW_OFFSET for calls to XDrawLine. OpenWindows has a problem
   that does not occur in generic X11R4. */

  XWindow *xw;
  UNUSED(ktext_length);

  xw = plot_window( CURRENT );

  if(bellON) {
    XBell(DISPLAY(xw),25);
  }
    
/* Select which events the plot window needs to accept for cursor input */

  XSelectInput(DISPLAY(xw),xw->win,
	       KeyPressMask        |

	       ButtonPressMask     |
               ButtonReleaseMask   |
	       EnterWindowMask     |
	       LeaveWindowMask     |
	       PointerMotionMask   |
	       StructureNotifyMask |
	       ExposureMask);


  expose3();

  /* While waiting for cursor text event, handle other events */

  cursortext_on3 = TRUE;

  dispatchevent3(&nerr);

/* Don't accept events anymore for cursor input */

  XSelectInput(DISPLAY(xw),xw->win,
               (StructureNotifyMask | ExposureMask));

/* Set location of cursor and character struck */

  *xloc_vp = x11_to_view_x(xcursor_p3, xw);
  *yloc_vp = x11_to_view_y(ycursor_p3, xw);

  strcpy(ktext, text_cursor3);

}

