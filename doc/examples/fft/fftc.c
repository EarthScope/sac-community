

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <assert.h>
#include <complex.h>

#include <sac.h>

void
check_dfft_parts(double *re, double *im, int n) {
    double re_true[16] = {0,0 ,0,0,0,0,0,0,0,0,0,0,0,0, 0,0};
    double im_true[16] = {0,0,-8,0,0,0,0,0,0,0,0,0,0,0,+8,0};
    for(int i = 0; i < n; i++) {
        if(fabs(re[i] - re_true[i]) >= 1e-10) {
            printf("%3d %15.8f %15.8f\n", i, re[i], re_true[i]);
            exit(-2);
        }
        if(fabs(im[i] - im_true[i]) >= 1e-10) {
            printf("%3d %15.8f %15.8f\n", i, im[i], im_true[i]);
            exit(-2);
        }

    }
}
void
check_fft_parts(float *re, float *im, int n) {
    float re_true[16] = {0,0 ,0,0,0,0,0,0,0,0,0,0,0,0, 0,0};
    float im_true[16] = {0,0,-8,0,0,0,0,0,0,0,0,0,0,0,+8,0};
    float tol = 1e-7;
    for(int i = 0; i < n; i++) {
        if(fabs(re[i] - re_true[i]) >= tol) {
            printf("%3d %15.8f %15.8f\n", i, re[i], re_true[i]);
            exit(-2);
        }
        if(fabs(im[i] - im_true[i]) >= tol) {
            printf("%3d %15.8f %15.8f\n", i, im[i], im_true[i]);
            exit(-2);
        }

    }
}
void
check_dfft_complex(double complex *z, int n) {
    double re[16], im[16];
    for(int i = 0; i < n; i++) {
        re[i] = creal(z[i]);
        im[i] = cimag(z[i]);
    }
    check_dfft_parts(re, im, n);
}
void
check_fft_complex(float complex *z, int n) {
    float re[16], im[16];
    for(int i = 0; i < n; i++) {
        re[i] = creal(z[i]);
        im[i] = cimag(z[i]);
    }
    check_fft_parts(re, im, n);
}

void
check_data(double *a, double *b, int n) {
    for(int i = 0; i < n; i++) {
        if(fabs(a[i] - b[i]) >= 1e-10) {
            printf("%3d %15.8f %15.8f\n", i, a[i], b[i]);
        }

    }
}
void
check_fdata(float *a, float *b, int n) {
    for(int i = 0; i < n; i++) {
        if(fabs(a[i] - b[i]) >= 1e-10) {
            printf("%3d %15.8f %15.8f\n", i, a[i], b[i]);
        }

    }
}

void
fft_double() {
    int n;
    int nf;
    double re[16], im[16], dt;
    double data[16], data2[16];
    double complex z[16];
    n = 16;
    nf = n;
    dt = 1.0;

    // Sine wave
    for(int i = 0; i < n; i++) {
        data[i] = sin((double) i * 2.0 * M_PI / ((double)n/2.0));
    }

    // FFT - Real Imaginary
    dfft(data, n, re, im, &nf);
    check_dfft_parts(re, im, nf);

    // FFT - Complex
    dfftz(data, n, z, &nf);
    check_dfft_complex(z, nf);

    // Inverse FFT - Real Complex
    idfft(data2, n, re, im, nf);
    check_data(data, data2, n);

    // Inverse FFT - Complex
    idfftz(data2, n, z, nf);
    check_data(data, data2, n);

}
void
fft_single() {
    int n;
    int nf;
    float re[16], im[16], dt;
    float data[16], data2[16];
    float complex z[16];
    n = 16;
    nf = n;
    dt = 1.0;

    // Sine wave
    for(int i = 0; i < n; i++) {
        data[i] = sin((float) i * 2.0 * M_PI / ((float)n/2.0));
    }
    // FFT - Real Imaginary
    fft(data, n, re, im, &nf);
    check_fft_parts(re, im, nf);

    // Inverse FFT - Real Complex
    ifft(data2, n, re, im, nf);
    check_fdata(data, data2, n);

    // FFT - Complex
    fftz(data, n, z, &nf);
    check_fft_complex(z, nf);

    // Inverse FFT - Complex
    ifftz(data2, n, z, nf);
    check_fdata(data, data2, n);
}

int
main() {
    fft_double();
    fft_single();
    return 0;

}
