/** 
 * @file getratio3.c
 *
 */

#include "gd3.x11.h"

/** 
 * Get the aspect ratio of the window 
 *
 * @param ratio
 *    Aspect ratio of the window on output
 *
 * @date   890608:  Used as-is from X10 version.  (kjm)
 * @date   870318:  Changes due to gd3.x10.h structure change.
 * @date   870223:  Allowed for multiple windows.
 *
 */
void 
getratio3(float *ratio) {

  XWindow *xw;

  xw = plot_window( CURRENT );

  /* Set the aspect ratio */
  *ratio = (float) xw->height / (float) xw->width;
}


