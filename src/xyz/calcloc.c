
#include <stdio.h>

#include "xyz.h"
#include "gdm.h"


#include "gd2.h"
#include "gd3.x11.h"

#include "debug.h"

void 
calcloc(unsigned int *xloc,
	unsigned int *yloc,
	unsigned int *cbarxoffset,
	unsigned int *cbaryoffset,
	unsigned int w_width,
	unsigned int w_height,
	unsigned int i_width,
	unsigned int i_height,
	float xpmn,
	float xpmx,
	float xmin,
	float first,
	float last,
	float ypmn,
	float ypdel,
	float vsratio,
	int *nerr)

{
  
        float x,y,cx,cy;

        int i, n;
        display_t **dev;

        UNUSED(vsratio);

        n   = gdm_get_ndevices();
        dev = gdm_get_devices();
        *nerr = 0;
        
        x = xpmn + (((xmin - first)/(last-first)*(xpmx-xpmn)));
        y = ypdel + ypmn;
        cx = 45;
        cy = 0;

        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->calc_loc) {
            dev[i]->calc_loc( &x, &y, &cx, &cy, w_width, w_height, i_width, i_height);
          }
        }

        *xloc = (int)x;
        *yloc = (int)y;
        *cbarxoffset = (int)cx;
        *cbaryoffset = (int)cy;

}






