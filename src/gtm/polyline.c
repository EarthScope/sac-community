
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gem.h"
#include "gtm.h"
#include "gdm.h"
#include "hdr.h"
#include "gam.h"

float data_to_view_x(float x);
float data_to_view_y(float y);

void 
polyline(float xloc[],
         float yloc[],
         int *number)
{
        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	/*=====================================================================
	 * PURPOSE:  To draw a line through a set of viewport locations.
	 *=====================================================================
	 * SPECIAL NOTE:  This polyline subroutine does NOT conform to the
	 *                SIGGRAPH standard in that it moves to the first
	 *                data point as opposed to drawing to it from the CP.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    xloc:    Array of x viewport coordinates. [fa]
	 *    yloc:    Array of y viewport coordinates. [fa]
	 *    number:  Length of xloc and yloc arrays. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *   saclib:   move, draw
	 *=====================================================================
	 * MODIFICATION HISTORYLOC:
	 *    910826:  Move declarations to after the include statements to 
	 *             improve portability to DEC 5000 per Gyu-sang Jang @ UC Davis.
	 *    910212:  add differential processing based on lvsclip;
	 *             add calls to drawpoly3/4 for X & SunView      (jjy)
	 *    831026:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861017
	 *===================================================================== */
	/* PROCEDURE: */
        for(i = 0; i < n; i++) {
            if(dev[i]->on && dev[i]->drawpoly ) {
                dev[i]->drawpoly( xloc, yloc, *number );
            }
        }
	return;

} /* end of function */

#define INSIDE  0
#define LEFT    1
#define RIGHT   2
#define BOTTOM  4
#define TOP     8

int
outcode(float x, float y, float r[4]) {
  int code = INSIDE;
  if(x < r[0]) {
    code |= LEFT;
  } else if(x > r[1]) {
    code |= RIGHT;
  }
  if(y < r[2]) {
    code |= BOTTOM;
  } else if(y > r[3]) {
    code |= TOP;
  }
  return code;
}

int 
clip_to_rect(float x[2], float y[2], float r[4]) {
  int c0,c1,c2;
  c0 = outcode(x[0],y[0],r);
  c1 = outcode(x[1],y[1],r);
  while(1) {
    if(!(c0 | c1)) {
      return 1;
    }
    if((c0 & c1)) {
      return 0;
    }
    double tx, ty;
    c2 = c0 ? c0 : c1;
    if(c2 & TOP) {
      tx = x[0] + (x[1] - x[0]) * (r[3] - y[0]) / (y[1] - y[0]);
      ty = r[3];
    } else if(c2 & BOTTOM) {
      tx = x[0] + (x[1] - x[0]) * (r[2] - y[0]) / (y[1] - y[0]);
      ty = r[2];
    } else if(c2 & RIGHT) {
      ty = y[0] + (y[1] - y[0]) * (r[1] - x[0]) / (x[1] - x[0]);
      tx = r[1];
    } else if(c2 & LEFT) {
      ty = y[0] + (y[1] - y[0]) * (r[0] - x[0]) / (x[1] - x[0]);
      tx = r[0];
    }

    if(c2 == c0) {
      x[0] = tx;
      y[0] = ty;
      c0 = outcode(x[0],y[0],r);
    } else {
      x[1] = tx;
      y[1] = ty;
      c1 = outcode(x[1],y[1],r);
    }
  }
  return 0;
}


#define SET_POINT(x,y,X,Y) do { \
    x = X;                      \
    y = Y;                      \
  } while(0);

void
polyfillrect(float *x, float *y, int n, int positive, float rect[4]) {
  int i, k;
  float xc[2],yc[2];
  float *xp, *yp, *tmp;
  int na;
  float base;
  
  int id, nd;
  display_t **dev;
  nd  = gdm_get_ndevices();
  dev = gdm_get_devices();
  

  base = (positive) ? rect[2] : rect[3];

  xc[0] = x[0];
  yc[0] = base;
  na = 16;
  xp = (float *) malloc(sizeof(float) * na);
  yp = (float *) malloc(sizeof(float) * na);
  k = 0;

  for(i = 0; i < n; i++) {
    xc[1] = x[i];
    yc[1] = y[i];
    
    if(clip_to_rect(xc, yc, rect)) {
      if(k+3 >= na) {
        na *= 2;
        if(!(tmp = (float *) realloc(xp, sizeof(float) * na))) {
          fprintf(stderr, "Error growing fill buffer\n");
          return;
        }
        xp = tmp;
        if(!(tmp = (float *) realloc(yp, sizeof(float) * na))) {
          fprintf(stderr, "Error growing fill buffer\n");
          return;
        }
        yp = tmp;
      }
      if(k == 0) {
        SET_POINT(xp[k], yp[k], xc[0], base); k++;
      }
      SET_POINT(xp[k], yp[k], xc[0], yc[0]); k++;
      SET_POINT(xp[k], yp[k], xc[1], yc[1]); k++;
    } else {
      if(k > 0) {
        SET_POINT(xp[k], yp[k], xp[k-1], base); k++;
        for(id = 0; id < nd; id++) {
          if(dev[id]->on && dev[id]->fillpoly ) {
            dev[id]->fillpoly( xp, yp, k );
          }
        }
        k = 0;
      }
    }
    xc[0] = x[i];
    yc[0] = y[i];
  }
  if(k > 0) {
    SET_POINT(xp[k], yp[k], xp[k-1], base); k++;
    for(id = 0; id < nd; id++) {
      if(dev[id]->on && dev[id]->fillpoly ) {
        dev[id]->fillpoly( xp, yp, k );
      }
    }
  }
  if(xp) {
    free(xp);
    xp = NULL;
  }
  if(yp) {
    free(yp);
    yp = NULL;
  }
}

void
polyfill(float *x, float *y, int n, int positive, int color) {
  int old;
    int yon, xon;
    float ymin,ymax,xmin,xmax;
    float rect[4] = {0,100,0,10};

    getylm(&yon, &ymin, &ymax);
    getxlm(&xon, &xmin, &xmax);

    if(positive) {
      ymin = fmax(ymin, 0);
      ymax = fmin(ymax, *depmax);
    } else {
      ymin = fmax(ymin, *depmin);
      ymax = fmin(ymax, 0);
    }

    old = cmgdm.icolor;
    setcolor( color );

    /* Convert data space to view space */
    xmin = data_to_view_x(xmin);
    xmax = data_to_view_x(xmax);
    ymin = data_to_view_y(ymin);
    ymax = data_to_view_y(ymax);

    /* Clipping rectangle limits in view space */
    rect[0] = fmax(xmin,cmgem.uplot.xmin);
    rect[1] = fmin(xmax,cmgem.uplot.xmax);
    rect[2] = fmax(ymin,cmgem.uplot.ymin);
    rect[3] = fmin(ymax,cmgem.uplot.ymax);

    polyfillrect(x, y, n, positive, rect);
    setcolor( old );
}
