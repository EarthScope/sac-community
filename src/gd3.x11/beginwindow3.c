/** 
 * @file beginwindow.c
 *
 */

#include <stdio.h>

#include "gd3.x11.h"
#include "bool.h"

/** 
 * Begin plotting to a specified window 
 *
 * @param win_num
 *    Window numeer
 * @param 
 *    Error Return Code
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date   890606:  Used as-is for X11 driver.  (kjm)
 * @date   870318:  Name changes due to gd3.x11.h structure change.
 * @date   870227:  Original Version
 */
void 
beginwindow3(int *win_num,
             int *nerr) {

  *nerr = 0;

/* Check if requested window is AVAILABLE */

  if (basew3[*win_num].status == AVAILABLE) 
    c_win3 = *win_num;
  else {
    fprintf(stderr, "Window number [%d] unavailable\n", *win_num);
    *nerr = 1;
  }

}



XWindow * 
plot_window(int num) {
  XWindow *r;
  r = NULL;
  if(num == CURRENT) {
    num = c_win3;
  }
  r = &plotw3[ num ];
  if(!r) {
    return NULL;
  }
  if(r->win == 0) {
    return NULL;
  }
  return r;
}

static int xwin_width  = -1;
static int xwin_height = -1;

void 
set_window_width_x11(int w)  {  
  xwin_width = w;  
}

void 
set_window_height_x11(int h) {  
  xwin_height = h; 
}

int is_window_size_set_in_pixels(void) {
  if(xwin_width > 0 && xwin_height > 0) {
    return TRUE;
  }
  return FALSE;
}

int 
get_window_width_x11(void)  { 
  return xwin_width; 
}

int 
get_window_height_x11(void) { 
  return xwin_height; 
}

int get_screen_width_x11(void)  { 
  XScreen *xs = xscreen_get();
  return xs->width;
}
int get_screen_height_x11(void) { 
  XScreen *xs = xscreen_get();
  return xs->height; 
}

static float plot_window_xy_ratio =  (11.0 / 8.5);
static int   constrain_plot_ratio_x11 = TRUE;

void
set_constrain_plot_ratio_x11(int set) {
    constrain_plot_ratio_x11 = set;
}

void
set_plot_ratio_x11(float ratio) {
    plot_window_xy_ratio = ratio;
}

void
get_window_size_x11(float *xmin,
                    float *xmax,
                    float *ymin,
                    float *ymax) {
  int sw, sh;
  /* w  = get_window_width_x11(); */
  /* h  = get_window_height_x11(); */
  sw = get_screen_width_x11();
  sh = get_screen_height_x11();
  if(constrain_plot_ratio_x11) {
      *xmax = *xmin + ( (*ymax - *ymin) * sh * plot_window_xy_ratio ) / sw;
      if(*xmax > 0.95) {
          *xmax = 0.95;
      }
  }
}
