
#include "gd2.h"
#include "gdm.h"
#include "debug.h"


GDM_EXTERN

void
calculate_location2(float *x, float *y, float *cx, float *cy, unsigned int w,
                    unsigned int h, unsigned int iw, unsigned int ih) {
    UNUSED(iw);
    *x = *x * XW;
    *y = (1.0 - *y - (float) ih / (float) h) * YW;
    *cx = (*cx / (float) w) * XW;
    *cy = ((ih - cmgdm.npscimage) / (float) h) * YW;

}

void
calc_loc2(unsigned int *xloc, unsigned int *yloc, unsigned int *cbarxoffset,
          unsigned int *cbaryoffset, unsigned int w_width,
          unsigned int w_height, unsigned int i_width, unsigned int i_height,
          double xpmn, double xpmx, double xmin, double first, double last,
          double ypmn, double ypdel, double vsratio, int *nerr)
{
    float xtemp, ypmx, ypmntemp;
    float proportion;
    *nerr = 0;
    UNUSED(vsratio);
    UNUSED(i_width);

    xtemp =
        (xpmn +
         (((xmin - first) / (last - first)) * (xpmx - xpmn))) * (float) w_width;
    *xloc = (xtemp / (float) w_width) * XW;

    ypmx = 1.0 - (ypmn + ypdel);
    ypmntemp = ypmx - (float) ((float) i_height / (float) w_height) - RNDOFF;
/*  *yloc = ypmntemp*vsratio*YW; */
    *yloc = ypmntemp * YW;

    *cbarxoffset = (45.0 / (float) w_width) * XW;
    proportion = (float) cmgdm.npscimage / (float) i_height;
    *cbaryoffset =
        ((i_height - (proportion * (float) i_height)) / (float) w_height) * YW;

    return;

}                               /* end of function */
