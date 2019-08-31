#include <stdio.h>
#include "scm.h"

void
rtrend(float *data, int n, float yint, float slope, double b, double delta) {
    int i;
    double t;
    for (i = 0; i < n; i++) {
        t = b + delta * i;
        data[i] = data[i] - yint - slope * t;
    }
}

void
rtrend2(float *data, int n, float yint, float slope, float *t) {
    int i;
    for (i = 0; i < n; i++) {
        data[i] = data[i] - yint - slope * t[i];
    }
}

void
rtrend_(float *data, int *n, float *yint, float *slope, double *b, double *delta) {
    rtrend(data, *n, *yint, *slope, *b, *delta);
}

void
rtrend__(float *data, int *n, float *yint, float *slope, double *b, double *delta) {
    rtrend(data, *n, *yint, *slope, *b, *delta);
}

void
rtrend2_(float *data, int *n, float *yint, float *slope, float *t) {
    rtrend2(data, *n, *yint, *slope, t);
}

void
rtrend2__(float *data, int *n, float *yint, float *slope, float *t) {
    rtrend2(data, *n, *yint, *slope, t);
}

/**
 *  Remove a linear trend from a data series
 *
 *  - data - Data Series
 *  - n - Length of data
 *  - delta - time sampling of data
 *
 *  Trend is removed in place and input data is overwritten
 *
 *  This calls lifite() and rtrend() internally
 *
 */
void
remove_trend(float *data, int n, float delta, float b) {
    float slope, yint, slope_sd, yint_sd, data_sd, corrcoef;
    /* Compute linear trend of the data */
    lifite(b, delta, data, n,
           &slope, &yint, &slope_sd, &yint_sd, &data_sd, &corrcoef);
    /* Remove linear trend from data */
    rtrend(data, n, yint, slope, b, delta);
}
void
remove_trend_(float *data, int *n, float *delta, float *b) {
    remove_trend(data, *n, *delta, *b);
}
void
remove_trend__(float *data, int *n, float *delta, float *b) {
    remove_trend(data, *n, *delta, *b);
}
