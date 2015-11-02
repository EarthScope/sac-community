
#include <math.h>
#include "scm.h"

void
interp(float *in, int nlen, float *out, int newlen, float bval, float eval,
       float dt, float tstart, float dtnew, float eps) {
    int j;
    float xnew;
    for (j = 0; j <= (newlen - 1); j++) {
        xnew = tstart + (j * dtnew);
        if (xnew >= bval && xnew <= eval) {
            wigint((float *) &bval, in, nlen, dt, eps, xnew, &out[j]);
        }
    }
}

void
interp2(float *in, int nlen, float *out, int newlen, float bval, float eval,
        float *t, float tstart, float dtnew, float eps) {
    int j;
    float xnew;
    for (j = 0; j < newlen; j++) {
        xnew = tstart + (j * dtnew);
        if (xnew >= bval && xnew <= eval) {
            wigint(t, in, nlen, 0.0, eps, xnew, &out[j]);
        }
    }
}


float
geteps(float y[], int nlen, float dx) {
    float avrat, eps;
    int j;

    /*  Calculate epsilon */
    avrat = 0.0;
    for (j = 0; j < (nlen - 1); j++) {
        avrat = avrat + fabs((y[j + 1] - y[j]) / dx);
    }
    eps = 0.0001 * avrat / (nlen - 1);
    return (eps);
}

float
geteps_xy(float y[], int nlen, float x[]) {
    float avrat, eps;
    int j;

    /*  Calculate epsilon */
    avrat = 0.0;
    for (j = 0; j <= (nlen - 1); j++) {
        avrat = avrat + fabs((y[j + 1] - y[j]) / (x[j + 1] - x[j]));
    }
    eps = 0.0001 * avrat / (nlen - 1);
    return (eps);
}


void
interp_(float *in, int *nlen, float *out, int *newlen, float *bval, float *eval,
        float *dt, float *tstart, float *dtnew, float *eps) {
    interp(in, *nlen, out, *newlen, *bval, *eval, *dt, *tstart, *dtnew, *eps);
}

void
interp__(float *in, int *nlen, float *out, int *newlen, float *bval,
         float *eval, float *dt, float *tstart, float *dtnew, float *eps) {
    interp(in, *nlen, out, *newlen, *bval, *eval, *dt, *tstart, *dtnew, *eps);
}

void
interp2_(float *in, int *nlen, float *out, int *newlen, float *bval,
         float *eval, float *t, float *tstart, float *dtnew, float *eps) {
    interp2(in, *nlen, out, *newlen, *bval, *eval, t, *tstart, *dtnew, *eps);
}

void
interp2__(float *in, int *nlen, float *out, int *newlen, float *bval,
          float *eval, float *t, float *tstart, float *dtnew, float *eps) {
    interp2(in, *nlen, out, *newlen, *bval, *eval, t, *tstart, *dtnew, *eps);
}

void geteps_ (float y[], int *nlen, float *dx, float *eps) {
    *eps = geteps(y,*nlen,*dx);
}
void geteps__(float y[], int *nlen, float *dx, float *eps) {
    *eps = geteps(y,*nlen,*dx);
}

void geteps_xy_ (float y[], int *nlen, float x[], float *eps) {
    *eps = geteps_xy(y,*nlen,x);
}
void geteps_xy__(float y[], int *nlen, float x[], float *eps) {
    *eps = geteps_xy(y,*nlen,x);
}

