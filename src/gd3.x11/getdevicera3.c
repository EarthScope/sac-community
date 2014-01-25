/** 
 * @file getdevicera3.c
 *
 */

#include "gd3.x11.h"

/** 
 *  To get the aspect ratio of the device.
 * 
 * @param ratio
 *    Device aspect ratio on output
 *
 * @date   890608:  Modified to run under X11 rather than X10.  (kjm)
 * @date   870227:  Original Version
 *
 */
void 
getdevicerat3(float *ratio)
{
  XScreen *xs;
  
  xs = xscreen_get();

  *ratio = ((float) xs->height) / ((float) xs->width);
  *ratio = 1.0;
}

