
#include <stdio.h>
#include "gd3.x11.h"
#include "debug.h"

void
calculate_location3(float *x, float *y, float *cx, float *cy, unsigned int w,
                    unsigned int h, unsigned int iw, unsigned int ih) {
    UNUSED(ih);
    UNUSED(iw);
    UNUSED(cx);
    UNUSED(cy);
    *x = *x * (float) w;
    *y = *y * (float) h;
}

void
calc_loc3(unsigned int *xloc, unsigned int *yloc, unsigned int *cbarxoffset,
          unsigned int *cbaryoffset, unsigned int w_width,
          unsigned int w_height, double xpmn, double xpmx, double xmin,
          double first, double last, double ypmn, double ypdel, int *nerr)
{
    *nerr = 0;

    *xloc =
        (xpmn +
         (((xmin - first) / (last - first)) * (xpmx - xpmn))) * (float) w_width;
    *yloc = (ypdel * (float) w_height) + ypmn * (float) w_height;
    *cbarxoffset = 45;
    *cbaryoffset = 0;

    return;

}                               /* end of function */
