/**
 * @file   td_conv.c
 *
 * @brief  Compute convolution of a long series (waveform) with pulse
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 * @param waveform
 *    Array containing input time series
 * @param n_w
 *    Number of samples in waveform
 * @param pulse
 *    Array containing the input pulse time series
 * @param n_p
 *    Number of samples in pulse
 * @param conv
 *    Array containing the output time series conv
 * @param n_conv
 *    Number of samples in conv
 * @param delta
 *    Time interval for waveform, pulse, conv
 * @param b_p
 *    begin time of pulse time series
 *
 * @return Nothing
 *
 */
static void td_conv(float     *waveform,
                    int        n_w,
                    float     *pulse,
                    int        n_p,
                    float     *conv,
                    float      delta,
                    float      factor,
                    float      b_p) {
    int i, j, j_1;
    float temp;

    if (n_p >= n_w) {
        fprintf(stderr, "Error: waveform and pulse lengths %d %d\n", n_w, n_p);
        exit(-1);
    }

    j_1 = -lrint(b_p/delta);

    for(i=0; i < n_w+n_p-1; i++){
        temp = 0.0;
        for(j=0; j < n_w; j++) {
            if (i >= (j-j_1) && n_p > (i-j+j_1)) {
                temp = temp + waveform[j]*pulse[i-j+j_1];
            }
        }
        conv[i] = factor*temp;
    }
    return;
}


/*
  convolvec.c: A time-series convolution
  Reads in a (short) pulse that is convolved with a (longer)
  waveform.  Lengths are n_p and n_w.  Output (conv)
  has length n_w + n_p + 1.  delta must be same for both.
  Easiily expanded to read in multiple long time series.

  \author   Arthur Snoke
  VT

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sacio.h>

#define MAX        10000
#define ERROR_MAX  256


int
main(int argc, char *argv[]) {

    /* Local variables */
    int i, j, j_1;
    int n_w, n_p, n_conv, nerr, max;

    float b_w, b_p, delta, deltaw, b_p_in, factor;
    char *wf_name, *p_name, *c_name, *disc_conv;

    float waveform[MAX], pulse[MAX], conv[MAX], dummy[MAX];

    char error[ERROR_MAX];

    if (argc == 1) {
        fprintf(stderr, "Usage: convolvec p_name wf_name c_name disc_conv\n");
        fprintf(stderr, "  where the first three arguments are filenames\n");
        fprintf(stderr, "  for pulse, waveform, and convolution output.\n");
        fprintf(stderr, "If disc_conv is y, it uses a discrete convolution\n");
        fprintf(stderr, "  and the pulse begin time is set to zero.\n");
        fprintf(stderr, "If disc_conv is n, pulse begin time is unchanged\n");
        fprintf(stderr, "  and the output is multiolied by delta, which is\n");
        fprintf(stderr, "  what one has in a time-series convolution.\n");
        exit(-1);
    }
    p_name = argv[1];
    wf_name = argv[2];
    c_name = argv[3];
    disc_conv = argv[4];

    max = MAX;

    for(i = 0; i < MAX; i++) {
        pulse[i] = 0.0;
    }

    /* Read in the pulse time series  */

    rsac1(p_name, pulse, &n_p, &b_p, &delta, &max, &nerr, SAC_STRING_LENGTH);

    if (nerr != 0) {
        fprintf(stderr, "Error reading in file(%d): %s\n", nerr, p_name);
        exit(-1);
    }

    /*  Test if want to do a discrete convolution*/

    factor = delta;
    b_p_in = b_p;
    if (disc_conv[0] == 'y') {
        factor = 1.0;
        b_p_in = 0.0;
    }

    /* If wanted to do more than one waveform, do loop starts here */

    for(i = 0; i < MAX; i++) {
        waveform[i] = 0.0;
        conv[i] = 0.0;
        dummy[i] = 0.0;
    }

    /* Read in the waveform time series */

    rsac1(wf_name, waveform, &n_w, &b_w, &deltaw, &max, &nerr, SAC_STRING_LENGTH);

    if (nerr != 0) {
        fprintf(stderr, "Error reading in file: %s\n", wf_name);
        exit(-1);
    }
  
     if (fabs(delta-deltaw) > 1.0e-4) {
        fprintf(stderr, "Delta p .ne. Delta w: %f, %f\n", delta, deltaw);
        exit(-1);
    }

    td_conv(waveform,n_w,pulse,n_p,conv,delta,factor,b_p_in);

    n_conv = n_w+n_p-1;
    setnhv ( "npts",   &n_conv,    &nerr, SAC_STRING_LENGTH);
    setkhv ( "kevnm",  "CONVOLUTION", &nerr, SAC_STRING_LENGTH, SAC_STRING_LENGTH);

    /* Write output SAC file */

    wsac0(c_name, dummy, conv, &nerr, SAC_STRING_LENGTH);
    if (nerr != 0) {
        fprintf(stderr, "Error writing out file: %s\n", c_name);
        exit(-1);
    }

    return 0;
}
