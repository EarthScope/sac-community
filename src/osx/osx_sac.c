
#include <stdio.h>
#include <stdlib.h>

#include "NSSacView_C.h"

static void *NSSac_id = NULL;
static void *SAC_id = NULL;

/* Set IDs */
void
osx_sac_nsview(void *id) {
    NSSac_id = id;
}

void
osx_sac(void *id) {
    SAC_id = id;
}

/* Window routines - SAC_id */
void
osx_sac_create_window(int n) {
    //printf("osx_sac_create_window: %p %d\n", SAC_id, n);
    sac_create_window(SAC_id, n);
}

void
osx_sac_focus_window(int n) {
    sac_focus_window(SAC_id, n);
}

int
osx_sac_find_window(int n) {
    return sac_find_window(SAC_id, n);
}

/* Plotting functions - NSSac_id */
void
osx_sac_flush() {
    NSSacView_update(NSSac_id);
}

void
osx_sac_mds(int type, float x, float y) {
    NSSacView_mds(NSSac_id, type, x, y);
}

void
osx_sac_size(int *width, int *height) {
    NSSacView_size(NSSac_id, width, height);
}

void
osx_sac_poly(int n, float *x, float *y) {
    NSSacView_poly(NSSac_id, n, x, y);
}

void
osx_sac_color(float r, float g, float b) {
    NSSacView_color(NSSac_id, r, g, b);
}

void
osx_sac_width(int width) {
    NSSacView_width(NSSac_id, width);
}

void
osx_sac_wait_for_keypress(float *x, float *y, char c[]) {
    NSSacView_wait_for_key_press(NSSac_id, x, y, c);
}

float
osx_sac_view_to_osx_x(float x) {
    int w, h;
    osx_sac_size(&w, &h);
    return x * (float) w;
}

float
osx_sac_view_to_osx_y(float y) {
    int w, h;
    osx_sac_size(&w, &h);
    return y * (float) w;
}

float
osx_sac_osx_to_view_x(float x) {
    int w, h;
    osx_sac_size(&w, &h);
    return x / (float) w;
}

float
osx_sac_osx_to_view_y(float y) {
    int w, h;
    osx_sac_size(&w, &h);
    return y / (float) w;
}

void
osx_sac_show_image(float *data, unsigned int iw, unsigned int ih, float xmin,
                   float xmax, float ymin, float ymax, float x, float y,
                   float w, float h, int npseudocolors, int nsacolors,
                   int ndefcolors, int lbinary) {
    NSSacView_show_image(NSSac_id, data, iw, ih, xmin, xmax, ymin, ymax, x, y,
                         w, h, npseudocolors, nsacolors, ndefcolors, lbinary);
}
