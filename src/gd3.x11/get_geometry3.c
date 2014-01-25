/** 
 * @file get_geometry3.c
 * 
 */

#include "gd3.x11.h"

/** 
 * Get geometry of the active window 
 *
 * @param window
 *    Window to get geometry of 
 * @param width_return
 *    Window Width 
 * @param height_return
 *    Window Height
 * @param nerr
 *    Error Return Code
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date   940823:  Original Version
 *
 */
void 
get_geometry3(int window,
              unsigned int *width_return,
              unsigned int *height_return,
              int *nerr)
{
  XWindow *xw;

  xw = plot_window( window );
  
  *nerr = 0;
  *width_return  = xw->width;
  *height_return = xw->height;

}
