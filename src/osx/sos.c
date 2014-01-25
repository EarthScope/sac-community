
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dbh.h"
#include "hdr.h"
#include "dff.h"
#include "dfm.h"
#include "bool.h"

typedef struct _complex complex;
struct _complex {
    float r, i;
};

complex 
cdiv(complex a, complex b) {
    float d;
    complex z;
    d = b.r * b.r + b.i * b.i;
    z.r = (a.r * b.r + a.i * b.i) / d;
    z.i = (a.i * b.r - a.r * b.i) / d;
    return z;
}

complex 
cinit(float r, float i) {
    complex z;
    z.r = r;
    z.i = i;
    return z;
}

complex
cmulf(complex a, float b) {
    complex z;
    z.r = a.r * b;
    z.i = a.i * b;
    return z;
}

complex
cmul(complex a, complex b) {
    complex z;
    z.r = a.r * b.r - a.i * b.i;
    z.i = a.i * b.r + a.r * b.i;
    return z;
}

complex
caddf(complex a, float b) {
    complex z;
    z.r = a.r + b;
    z.i = a.i;
    return z;
}

complex 
polyval(int n, float *p, complex z) {
    int i;
    complex y;
    y = cinit(0.0, 0.0);
    for(i = 0; i < n; i++) {
        y = caddf( cmul(y, z), p[i] );
    }
    return y;
}

complex
cexpo(complex a) {
    complex z;
    z.r = exp(a.r) * cos(a.i);
    z.i = exp(a.r) * sin(a.i);
    return z;
}

complex *
freqz(int    n,
      float *b,
      float *a,
      int    nin) {

    int i;
    float dw;
    complex *z, zml;

    dw = M_PI/nin;
    z = (complex *) malloc(sizeof(complex) * nin);
    for(i = 0; i < nin; i++) {
        zml = cexpo( cmulf( cinit(0.0, -1.0), i * dw));
        z[i] = cdiv( polyval(n,b,zml), polyval(n,a,zml) );
    }
    return z;
}

float 
cabso(complex a) {
    return sqrt(a.r * a.r + a.i * a.i);
}
float
cphase(complex a) {
    return atan2(a.i, a.r);
}

void
sos_freqz(int ns, 
          float *sn,
          float *sd,
          float  dt,
          int    n,
          float *freq,
          float *amp,
          float *phase) {
    complex *z, *H ;
    int i, j;
    
    H = (complex*) malloc(sizeof(complex) * n);
    for(i = 0; i < ns; i++) {
        z = freqz(3, &sn[i*3], &sd[i*3], n);
        if(i == 0) {
            for(j = 0; j < n; j++) {
                H[j].r = z[j].r;
                H[j].i = z[j].i;
            }
        } else {
            for(j = 0; j < n; j++) {
                H[j] = cmul(H[j], z[j]);
            }
        }
        free(z);
    }
    for(i = 0; i < n; i++) {
        freq[i]  = i * ((1.0 / dt / 2.0)/n);
        amp[i]   = cabso(H[i]);
        phase[i] = cphase(H[i]);
    }
}

void
filter_response(char   *type, 
                char   *proto,
                int     order,
                float   low,
                float   high,
                double  dt,
                int     n,
                float  *freq,
                float  *amp,
                float  *phase) {
    int nsects;
    float sd[30], sn[30];
    design(order, 
           type, proto, 
           0.0, 0.0,
           low, high,
           dt,
           sn, sd, &nsects);
    sos_freqz(nsects, sn, sd, dt, n, freq, amp, phase);
}

float
max_delta() {
    int i, n;
    int nlen, index1, index2, nerr;
    float dt = 0.0;
    n = cmdfm.ndfl;
    for(i = 0; i < n; i++) {
        getfil(i+1, TRUE, &nlen, &index1, &index2, &nerr);
        dt = fmax(dt, *delta);
    }
    return dt;
}

int
max_npts() {
    int i, n;
    int nlen, index1, index2, nerr;
    int npts = 0.0;
    n = cmdfm.ndfl;
    for(i = 0; i < n; i++) {
        getfil(i+1, TRUE, &nlen, &index1, &index2, &nerr);
        npts = (npts > nlen) ? npts : nlen;
    }
    return npts;
}
