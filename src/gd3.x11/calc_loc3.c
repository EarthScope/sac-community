
#include <stdio.h>
#include "gd3.x11.h"
#include "debug.h"

void 
calculate_location3(float *x,
                    float *y,
                    float *cx,
                    float *cy,
                    unsigned int w,
                    unsigned int h,
                    unsigned int iw,
                    unsigned int ih) {
  UNUSED(ih);
  UNUSED(iw);
  UNUSED(cx);
  UNUSED(cy);  
  *x = *x * (float)w;
  *y = *y * (float)h;
}

void calc_loc3(xloc,yloc,cbarxoffset,cbaryoffset,w_width,w_height,xpmn,xpmx,xmin,first,last,ypmn,ypdel,nerr)
unsigned int *xloc, *yloc, *cbarxoffset, *cbaryoffset;
unsigned int w_width, w_height;
float xpmn,xpmx,xmin,first,last,ypmn,ypdel;
int *nerr;
{
      *nerr = 0;

      *xloc = (xpmn + (((xmin-first)/(last-first))*(xpmx-xpmn)))*(float)w_width;
      *yloc = (ypdel * (float)w_height)+ypmn*(float)w_height;
      *cbarxoffset = 45;
      *cbaryoffset = 0;

       
	return ;

} /* end of function */






