/** 
 * @file beginwindow.c
 *
 */

#include <stdio.h>

#include "gd3.x11.h"
#include "bool.h"

extern XWindow basew3[MAX_WINS + 1];
extern XWindow plotw3[MAX_WINS + 1];

GD3_EXTERN

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
beginwindow3(int *win_num, int *nerr) {

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
    if (num == CURRENT) {
        num = c_win3;
    }
    r = &plotw3[num];
    if (!r) {
        return NULL;
    }
    if (r->win == 0) {
        return NULL;
    }
    return r;
}

int
get_screen_width_x11(void) {
    XScreen *xs = xscreen_get();
    return xs->width;
}

int
get_screen_height_x11(void) {
    XScreen *xs = xscreen_get();
    return xs->height;
}

static float plot_window_xy_ratio = (11.0 / 8.5);
static int constrain_plot_ratio_x11 = TRUE;

void
set_constrain_plot_ratio_x11(int set) {
    constrain_plot_ratio_x11 = set;
}

void
set_plot_ratio_x11(float ratio) {
    plot_window_xy_ratio = ratio;
}

void
get_window_size_x11(float *xmin, float *xmax,
                    float *ymin, float *ymax,
                    int use_ratio, float ratio) {
    int sw, sh;
    float sr;
    /* w  = get_window_width_x11(); */
    /* h  = get_window_height_x11(); */
    sw = get_screen_width_x11();
    sh = get_screen_height_x11();
    sr = (float) sw / sh;
    // Check if Aspect ratio is off and screen is ultra-wide (> 2.0 or < 0.5 )
    //   e.g. Multiple monitors and "high-end displays"
    //   Turn Aspect ratio on and set to Letter Size Paper
    if( !use_ratio && (sr > 2.0 || sr < 0.5)) {
        use_ratio = TRUE;
        ratio = 11 / 8.5;
    }
    if (use_ratio) {
        *xmax = *xmin + ((*ymax - *ymin) * sh * ratio) / sw;
        if (*xmax > 0.95) {
            *xmax = 0.95;
        }
    }
}
