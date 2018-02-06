/** 
 * @file   crscor.c
 * 
 * @brief  Compute Cross-Correlation Function
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbh.h"
#include "co.h"
#include "ucf.h"

struct t_big {
    float *caux;
    float *w;
    float *workr;
    float *worki;
} big;

/** 
 * Compute the Cross-Correlation Function
 * 
 * @param data1 
 *    Array containing the first data sequence
 * @param data2 
 *    Array containing the second data sequence
 * @param nsamps 
 *    Number of samples in data sequence
 * @param nwin 
 *    Requested Number of windows
 * @param wlen 
 *    Requested number of samples in each window.  The 
 *    subroutine will calculate the window overlap. 
 *    Maximum value is 2048
 * @param type 
 *    Type of data analysis window to use.  Valid values
 *    are:
 *     -   <HAM>MING
 *     -   <HAN>NING
 *     -   <C>OSINE
 *     -   <R>ECTAN
 *     -   <T>RIANG
 * @param c 
 *    Output Array containing resulting 2*\p wlen -1 length 
 *    correlation coefficients.  The correlation sequence is 
 *    circularly rotated in the array so that the zeroth lag 
 *    is at the beginning.  Array dimensions 0:4095
 * @param nfft 
 *    Number of samples in the correlation sequence.  
 *    May be padded with zeros.
 * @param err 
 *    Error Message
 * @param err_s 
 *    Length of string \p err
 *
 * @return Nothing
 *
 * \author   Dave Harris
 *           L-205
 *           Lawrence Livermore National Laboratory
 *           Livermore, Ca  94550
 *
 * \date 800130  Created
 * \date 840621  Last Modified
 *
 *
 */
void
crscor(float *data1, float *data2, int nsamps, int nwin, int wlen, char *type,
       float *c, int *nfft, char *err, int err_s) {
    char temp[131];
    int half, i, j, k, lsamp, nlags, nverlp, point;
    float scale, scale1, scale2, xi, xr, yi, yr;

    float *const Data1 = &data1[0] - 1;
    float *const Data2 = &data2[0] - 1;

    /*  Initializations
     * */
    fstrncpy(err, err_s - 1, " ", 1);

    /*  Check for legal window length and compute overlap
     * */
    nlags = 2 * wlen - 1;
    if (nwin < 1) {

        fstrncpy(err, err_s - 1, " CRSCOR - too few windows ", 26);
        return;

    } else if (wlen < 1 || wlen > nsamps) {

        fstrncpy(err, err_s - 1, " CRSCOR - illegal window length ", 32);
        return;

    } else {

        /*                                               Everything OK */

        if (nwin * wlen <= nsamps) {
            nverlp = 0;
        } else {
            nverlp = (nwin * wlen - nsamps) / (nwin - 1);
            if (nwin * wlen - nverlp * (nwin - 1) > nsamps) {
                nverlp = nverlp + 1;
            }
        }
        lsamp = wlen - 1;

    }

    /*  Find first power of two >= #LAGS
     * */
    *nfft = 8;
  L_2:
    ;
    if (*nfft >= nlags)
        goto L_3;
    *nfft = *nfft * 2;
    goto L_2;
  L_3:
    ;
    half = *nfft / 2;

    if ((big.w = (float *) malloc(wlen * sizeof(float))) == NULL) {
        printf("memory allocation failed in crscor\n");
        goto L_8892;
    }

    if ((big.caux = (float *) malloc(*nfft * sizeof(float))) == NULL) {
        printf("memory allocation failed in crscor\n");
        goto L_8891;
    }

    if ((big.workr = (float *) malloc(*nfft * sizeof(float))) == NULL) {
        printf("memory allocation failed in crscor\n");
        goto L_8890;
    }

    if ((big.worki = (float *) malloc(*nfft * sizeof(float))) == NULL) {
        printf("memory allocation failed in crscor\n");
        goto L_8889;
    }

    /*  Generate window
     * */
    for (i = 0; i <= lsamp; i++) {
        big.w[i] = 1.;
        /*             I */
    }
    window(&big.w[0], wlen, type, 1, wlen, &big.w[0], err, err_s);

    /*  Check validity of window calculation
     * */
    if (memcmp(err, "        ", 8) != 0) {
        fstrncpy(temp, 130, err, strlen(err));
        fstrncpy(temp + strlen(err), 130 - strlen(err), " (from CROSS)", 13);
        fstrncpy(err, err_s - 1, temp, strlen(temp));
        goto L_8888;
    }

    /*  Compute cross-correlation function
     *
     *
     *    Initialize window pointer
     * */
    point = 1;

    /*    Initialize correlation arrays
     * */
    zero(&c[0], *nfft);
    zero(&big.caux[0], *nfft);

    /*    Compute cross-spectrum for each window,  then average
     * */
    for (i = 1; i <= nwin; i++) {

        /*    Zero work arrays
         * */
        zero(&big.workr[0], *nfft);
        zero(&big.worki[0], *nfft);

        /*    Load data into arrays
         * */
        /* copy( (int*)&Data1[point], (int*)&big.workr[0], wlen ); */
        /* copy( (int*)&Data2[point], (int*)&big.worki[0], wlen ); */

        copy_float(&(Data1[point]), big.workr, wlen);
        copy_float(&(Data2[point]), big.worki, wlen);

        /*    Compute scale factors
         * */
        scale1 = rms(&big.workr[0], wlen);
        scale2 = rms(&big.worki[0], wlen);
        scale = scale1 * scale2;

        /*    Window and scale data
         * */
        for (j = 0; j <= lsamp; j++) {
            big.workr[j] = big.workr[j] * big.w[j] / scale1;
            big.worki[j] = big.worki[j] * big.w[j] / scale2;
            /*               J */
        }

        /*    Compute and average cross spectra
         * */
        fft_dbh(&big.workr[0], &big.worki[0], *nfft, -1);

        /*      Special case for point at 0
         * */
        c[0] = c[0] + big.workr[0] * big.worki[0] * scale;

        /*      All other points
         * */
        for (j = 1; j <= half; j++) {

            k = *nfft - j;

            xr = (big.workr[j] + big.workr[k]) * .5;
            xi = (big.worki[j] - big.worki[k]) * .5;
            yr = (big.worki[j] + big.worki[k]) * .5;
            yi = (big.workr[k] - big.workr[j]) * .5;

            c[j] = c[j] + (xr * yr + xi * yi) * scale;
            big.caux[j] = big.caux[j] + (xr * yi - xi * yr) * scale;
            c[k] = c[j];
            big.caux[k] = -big.caux[j];

        }

        /*    Update window pointer
         * */
        point = point + wlen - nverlp;

    }

    /*    Inverse fft for correlation computation
     * */
    fft_dbh(&c[0], &big.caux[0], *nfft, 1);

    /*  Bye
     * */

  L_8888:
    free(big.worki);

  L_8889:
    free(big.workr);

  L_8890:
    free(big.caux);

  L_8891:
    free(big.w);

  L_8892:
    return;
}

void
cross_correlation_normalized(float *data1, float *data2, int *npts_data,
                             float *xcorr, int *npts_xcorr) {
    int j;
    float squared_sum_master;
    float squared_sum_slave;
    float demon;

    squared_sum_master = 0.0;
    squared_sum_slave = 0.0;
    for (j = 0; j < *npts_data; j++) {
        squared_sum_master += data1[j] * data1[j];
        squared_sum_slave += data2[j] * data2[j];
    }
    demon = sqrt(squared_sum_master * squared_sum_slave);

    for (j = 0; j < *npts_xcorr; j++) {
        xcorr[j] = xcorr[j] / demon;
    }
}

void
crscor_(float *data1, float *data2, int *nsamps, int *nwin, int *wlen,
        char *type, float *c, int *nfft, char *err, int err_s) {
    crscor(data1, data2, *nsamps, *nwin, *wlen, type, c, nfft, err, err_s);
}

void
crscor__(float *data1, float *data2, int *nsamps, int *nwin, int *wlen,
         char *type, float *c, int *nfft, char *err, int err_s) {
    crscor(data1, data2, *nsamps, *nwin, *wlen, type, c, nfft, err, err_s);
}

#define UNUSED(x) (void)(x)

/**
 *  Compute the maximum value of an array
 *
 *  - c - float array (returned from correlate function)
 *  - nc - length of c
 *
 *  Return: Index of maximum value in array
 */
int
correlate_max(float *c, int nc) {
    int i;
    float cmax = c[0];
    int imax   = 0;
    for(i = 1; i < nc; i++) {
        if(c[i] > cmax) {
            imax = i;
            cmax = c[i];
        }
    }
    return imax;
}

/**
 *  Compute the time of a data point given dt and begin time
 *
 *  - dt - Time sampling
 *  - b - Begin time
 *  - i - data sample
 *
 *  Return: time value (b + i * dt)
 */
float
correlate_time(float dt, float b, int i) {
    return b + i * dt;
}

/**
 *  Compute a time array given dt and begin time
 *
 *  - dt - Time sampling
 *  - b - Begin time
 *  - n - Length of data array
 *
 *  Return: time array 
 */
float *
correlate_time_array(float dt, float b, int n) {
    int i;
    float *t = calloc(n, sizeof(float));
    for(i = 0; i < n; i++) {
        t[i] = correlate_time(dt, b, i);
    }
    return t;
}
/**
 *  Compute begin time from a correlation of two time series
 *
 *  - dt - Time sampling
 *  - n1 - Length of first time series
 *  - n2 - Length of second time series (unused)
 *  - b1 - Begin time of first time series
 *  - b2 - Begin time of second time series
 *
 *  Return: -dt * (n1 - 1) + (b2-b1)
 */
float
correlate_time_begin(float dt, float n1, float _n2, float b1, float b2) {
    UNUSED(_n2);
    return -dt * (n1 - 1) + (b2 - b1);
}

#define ERROR_MAX 256


/**
 *  Compute the cross correlation function from two time series
 *
 *  - f - First time series
 *  - nf - Length of first time series, f
 *  - g - Second time series
 *  - ng - Length of second time series, g
 *  - c - Cross correlation time series
 *  - nc - Size of c, must be at least (nf + ng - 1)
 *
 *  Return: Cross correlation function, length: nf + ng - 1
 *
 *  If the signals are not the same length, then find the longest
 *  signal, make both signals that length by filling the remainder
 *  with zeros (pad at the end) and then run them through crscor
 *
 */
void
correlate(float *f, int nf, float *g, int ng, float *c, int nc) {

    float *f2, *g2;
    float *ytmp;
    int i;
    int nlen, nwin, wlen, nfft, max;

    char error[ERROR_MAX];

    if(nc < nf+ng-1) {
        printf("Correlate output not long enough\n");
        return;
    }
    
    if(nf >= ng) {
        nlen = nf;
    } else if(nf < ng) {
        nlen = ng;
    }

    /* Allocate space for the correlation of yarray1 and yarray2 */
    /* 2 * n - 1 */
    max = next2((2 * nlen) - 1) * 2;

    // Create Output array and temp work array
    //out  = (float *) calloc(max, sizeof(float));
    ytmp = (float *) calloc(max*4, sizeof(float));

    // Pad input data with zeros up to next power of 2
    nlen = next2(nlen);
    f2 = (float *) calloc(nlen, sizeof(float));
    g2 = (float *) calloc(nlen, sizeof(float));
    memcpy(f2, f, nf * sizeof(float));
    memcpy(g2, g, ng * sizeof(float));

    /* Set up values for the cross correlation */
    nwin = 1;
    wlen = nlen;
    nfft = 0;

    /*     Call crscor ( Cross Correlation )
     *        - yarray1 - First  Input array to correlate
     *        - yarray2 - Second Input array to correlate
     *        - nlen    - Number of points in yarray and yarray2
     *        - nwin    - Windows to use in the correlation
     *        - wlen    - Length of the windows
     *        - type    - Type of Window (SAC_RECTANGLE)
     *        - out     - output sequence
     *        - nfft    - Length of the output sequence
     *        - error   - Error Message
     *        - err_len - Length of Error Message (on input)
     */
    crscor(f2, g2, nlen, nwin, wlen, SAC_RECTANGLE, ytmp, &nfft, error, ERROR_MAX);

    /*
     *     out[0 : nlen1 - 2 ] <-- ytmp[ nfft - nlen1 + 1 : nfft -1 ]
     *     out[nlen1 - 1 : nlen1 + nlen2 - 2 ] <-- ytmp[ 0 : nlen2-1 ]
     */
    for(i = 0; i <= nf - 2; i++) {
      c[i] = ytmp[nfft - nf + i + 1];
    }
    for(i = 0; i <= ng - 1; i++) {
      c[nf - 1 + i ] = ytmp[i];
    }
}

void
correlate_(float *f, int *nf, float *g, int *ng, float *c, int *nc) {
    correlate(f, *nf, g, *ng, c, *nc);
}
void
correlate__(float *f, int *nf, float *g, int *ng, float *c, int *nc) {
    correlate(f, *nf, g, *ng, c, *nc);
}

float
correlate_time_begin_(float *dt, int *n1, int *n2, float *b1, float *b2) {
    return correlate_time_begin(*dt, *n1, *n2, *b1, *b2);
}
float
correlate_time_begin__(float *dt, int *n1, int *n2, float *b1, float *b2) {
    return correlate_time_begin(*dt, *n1, *n2, *b1, *b2);
}

int correlate_max_ (float *c, int *nc) { return correlate_max(c, *nc) + 1; }
int correlate_max__(float *c, int *nc) { return correlate_max(c, *nc) + 1; }
float
correlate_time_(float *dt, float *b, int *i) {
    return correlate_time(*dt, *b, *i - 1);
}
float
correlate_time__(float *dt, float *b, int *i) {
    return correlate_time(*dt, *b, *i - 1);
}


void
convolve(float *a, int na, float *b, int nb, float *c, int nc) {
    int i, j, nlen, nfft, nwin, wlen;
    float *a0, *out;
    char error[ERROR_MAX];

    if(nc < na + nb - 1) {
        printf("output length must be at least na + nb - 1\n");
        return;
    }

    a0 = calloc(na, sizeof(float));
    /* Reverse the First Signal */
    j = 0;
    for(i = na - 1; i >= 0; i--) {
      a0[j] = a[i];
      j++;
    }

    nlen = na;
    if(nb > nlen) {
        nlen = nb;
    }
    /* Allocate space for the correlation of yarray1 and yarray2 */
    nfft = next2((2 * nlen) - 1) * 2;
    out = calloc(nfft, sizeof(float));

    /* Set up values for the cross correlation */
    nwin = 1;
    wlen = nlen;
    nfft = 0;

    /*     Call crscor ( Cross Correlation, no, wait, uh Convolution )
     *        - yarray1 - First  Input array to correlate
     *        - yarray2 - Second Input array to correlate
     *        - nlen    - Number of points in yarray and yarray2
     *        - nwin    - Windows to use in the correlation
     *        - wlen    - Length of the windows
     *        - type    - Type of Window (SAC_RECTANGLE)
     *        - out     - output sequence
     *        - nfft    - Length of the output sequence
     *        - error   - Error Message
     *        - err_len - Length of Error Message (on input)
     */
    crscor(a0, b, nlen,
           nwin, wlen, SAC_RECTANGLE,
           out, &nfft, error, ERROR_MAX);

    /* Reconstruct the signal from the "cross correlation" back to front
     *
     *  ytmp[0         : nlen1 - 2         ] <- out[nfft-nlen1+1 : nfft  - 1 ] 
     *  ytmp[nlen1 - 1 : nlen1 + nlen2  -2 ] <- out[0            : nlen2 - 1 ]
     *
     *  nfft-1 is the last point of the output sequence
     */
    for(i = 0; i <= na - 2; i++) {
        c[i] = out[nfft - na + i + 1];
    }
    for(i = 0; i <= nb - 1; i++) {
        c[na + i - 1] = out[i];
    }

    free(out);
    free(a0);
}

void
convolve_(float *a, int *na, float *b, int *nb, float *c, int *nc) {
    convolve(a, *na, b, *nb, c, *nc);
}
void
convolve__(float *a, int *na, float *b, int *nb, float *c, int *nc) {
    convolve(a, *na, b, *nb, c, *nc);
}

