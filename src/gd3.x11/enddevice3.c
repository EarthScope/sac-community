/** 
 * @file enddevice3.c
 *
 */

#include "gd3.x11.h"

/** 
 * End X11 graphics device
 * 
 * @param nerr
 *    Error Return Code
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date   911031:  Marked the window status UNAVAILABLE. (wct)
 * @date   910819:  Changed XDestroyWindow to XCloseDisplay and commented
 *                  call to XUnloadFont (wct).
 * @date   890608:  Modified to run under X11 rather than X10.  (kjm)
 * @date   870318:  Changes due to gd3.x10.h structure change.
 * @date   870317:  Added call to free font.
 * @date   870223:  Original Version
 */
void 
enddevice3(int *nerr)
{
  int i;
  XScreen *xs;
 
  xs = xscreen_get();

  *nerr = 0;

  /* Destroy window activity and set status flag */
  c_win3 = 0;
  for (i=1; i <= num_wins3; i++) {
    basew3[i].status = UNAVAILABLE;
  }

  /*  Close window display */
  XCloseDisplay(xs->display);
  
}
