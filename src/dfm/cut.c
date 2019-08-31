
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "errors.h"
enum {
    CUT_FILLZ = 3,
    CUT_USEBE = 2,
    CUR_FATAL = 1,
};

void
cut_data(float *in, int nstart, int nstop, int nfillb, int nfille, float *out) {
    int out_offset, in_offset, n;

    /* Number of data points to cut */
    n = nstop - nstart + 1 - nfillb - nfille;

    /* Fill full array with zeros */
    memset(out, 0, sizeof(float) * nfillb + nfille + (nstop - nstart + 1));

    out_offset = nfillb;
    if (n > 0) {
        in_offset = nstart - 1 + nfillb;
        memmove(out + out_offset, in + in_offset, n * sizeof(float));
    }
}

/**
 *  Compute data point from time pick
 *
 *  - b - Begin value of data
 *  - delta - time sampling of data
 *  - dt - Timing value to convert to data point
 *  - n - Output - converted data point from timing value
 *
 *  From defcut - only acts on times, not picks
 */
void
cut_define(double b, double delta, double dt, int *n) {
    int iTime, iBegin;

    iBegin = lround((double)b / (double)delta);

    /* Compute time and index */
    iTime = lround((double)dt / (double)delta);
    *n = iTime - iBegin + 1;

}

void
cut_define_check(double start, double stop, int npts, int cuterr, int *nstart,
                 int *nstop, int *nfillb, int *nfille, int *nerr) {
    /* - Check that start value less than stop value. */
    *nerr = 0;
    if (start >= stop) {
        *nerr = ERROR_START_TIME_GREATER_THAN_STOP;
        return;
    }

    /* - Handle cases where the requested data window is not entirely
     *   within the range of the data file. */

    /* -- Entire data window after file end. */
    if (*nstart > npts) {
        if (cuterr == CUT_FILLZ) {
            *nfillb = 0;
            *nfille = *nstop - *nstart + 1;
        } else {
            *nerr = ERROR_START_TIME_GREATER_THAN_END;
        }
        return;
    }

    /* -- Entire data window before file begin. */
    if (*nstop < 1) {
        if (cuterr == CUT_FILLZ) {
            *nfillb = *nstop - *nstart + 1;
            *nfille = 0;
        } else {
            *nerr = ERROR_STOP_TIME_LESS_THAN_BEGIN;
        }
        return;
    }

    /* - Start of data window before file begin. */
    if (*nstart < 1) {
        if (cuterr == CUT_FILLZ) {
            *nfillb = 1 - *nstart;
        } else if (cuterr == CUT_USEBE) {
            *nerr = ERROR_START_TIME_LESS_THAN_BEGIN;
            *nstart = 1;
            *nfillb = 0;
        } else {
            *nerr = ERROR_START_TIME_LESS_THAN_BEGIN;
            return;
        }
    } else {
        *nfillb = 0;
    }

    /* -- Stop of data window is after file end. */
    if (*nstop > npts) {
        if (cuterr == CUT_FILLZ) {
            *nfille = *nstop - npts;
        } else if (cuterr == CUT_USEBE) {
            if (*nerr == ERROR_START_TIME_LESS_THAN_BEGIN) {
                *nerr = ERROR_CUT_TIMES_BEYOND_DATA_LIMITS;
            } else {
                *nerr = ERROR_STOP_TIME_GREATER_THAN_END;
            }
            *nstop = npts;
            *nfille = 0;
        } else {
            *nerr = ERROR_STOP_TIME_GREATER_THAN_END;
            return;
        }
    } else {
        *nfille = 0;
    }
}

void
cut_data_(float *in, int *nstart, int *nstop, int *nfillb, int *nfille, float *out) {
    cut_data(in, *nstart, *nstop, *nfillb, *nfille, out);
}

void
cut_data__(float *in, int *nstart, int *nstop, int *nfillb, int *nfille, float *out) {
    cut_data(in, *nstart, *nstop, *nfillb, *nfille, out);
}

void
cut_define_(double *b, double *delta, double *dt, int *n) {
    cut_define(*b, *delta, *dt, n);
}

void
cut_define__(double *b, double *delta, double *dt, int *n) {
    cut_define(*b, *delta, *dt, n);
}

void
cut_define_check_(double *start, double *stop, int *npts, int *cuterr,
                  int *nstart, int *nstop, int *nfillb, int *nfille,
                  int *nerr) {
    cut_define_check(*start, *stop, *npts, *cuterr, nstart, nstop, nfillb,
                     nfille, nerr);
}

void
cut_define_check__(double *start, double *stop, int *npts, int *cuterr,
                   int *nstart, int *nstop, int *nfillb, int *nfille,
                   int *nerr) {
    cut_define_check(*start, *stop, *npts, *cuterr, nstart, nstop, nfillb,
                     nfille, nerr);
}


void
cutd(float *y, int npts, double b, double dt,
    double begin_cut, double end_cut, int cuterr,
    float *out, int *nout) {
    int nfillb, nfille, nstart, nstop, nerr;

    // Determine start and stop data points for cut
    //
    //   Call cut_define
    //    - begin_cut    - Begin time for cut
    //    - dt           - Sample rate of data
    //    - end_cut      - End time for cut
    //    - npts_cut     - Number of points in data after cutting
    cut_define(b, dt, begin_cut, &nstart);
    cut_define(b, dt, end_cut, &nstop);

    // Determine data points (integers) to cut at
    //
    //   Call cut_define_check
    //    - begin_cut - Begin time for cut
    //    - end_cut   - End time for cut
    //    - npts      - Number of points in data
    //    - cuterr    - How to handle cuts outside the length of the trace.
    //              Three possible values:
    //                - CUT_FATAL = 1 throws an error if the cut window is too large
    //                - CUT_USEBE = 2 use the b and e values if the cut window is too large
    //                - CUT_FILLZ = 3 fills with zeros if the cut windows is too large
    //    - nstart    - Number of points corresponding to begin_cut
    //    - nstop     - Number of points corresponding to end_cut
    //    - nfillb    - Number of points filled before begin_time
    //    - nfille    - Number of pionts filled after end_time
    //    - nerr      - Error value returned
    cut_define_check(begin_cut, end_cut, npts, cuterr,  // Input
                     &nstart, &nstop,                   // Input / Output
                     &nfillb, &nfille, &nerr);          // Outputs

    if(*nout < nstop - nstart + 1) {
        printf("output for cut not long enough\n");
        return;
    }
    *nout = nstop - nstart + 1;
    
    // Check errors

    // Do the actual cutting
    //   Call cut
    //   - data       - Original data to cut
    //   - nstart     - Number of points corresponding to begin_cut
    //   - nstop      - Number of points corresponding to end_cut
    //   - nfillb     - Number of points filled with zeros if begin_time is before data
    //   - nfille     - Number of points filled with zeros if end_time is after data
    //   - cut_data   - Cut data
    cut_data(y, nstart, nstop, nfillb, nfille, out);
}

void
cutd_(float *y, int *npts, double *b, double *dt,
     double *begin_cut, double *end_cut, int *cuterr,
     float *out, int *nout) {
    cutd(y, *npts, *b, *dt, *begin_cut, *end_cut, *cuterr, out, nout);
}
void
cutd__(float *y, int *npts, double *b, double *dt,
      double *begin_cut, double *end_cut, int *cuterr,
      float *out, int *nout) {
    cutd(y, *npts, *b, *dt, *begin_cut, *end_cut, *cuterr, out, nout);
}

void
cut(float *y, int npts, float b, float dt,
     float begin_cut, float end_cut, int cuterr,
     float *out, int *nout) {
    cutd(y, npts, (double) b, (double) dt, (double) begin_cut, (double) end_cut,
         cuterr, out, nout);
}
void
cut_(float *y, int *npts, float *b, float *dt,
     float *begin_cut, float *end_cut, int *cuterr,
     float *out, int *nout) {
    cutd(y, *npts, (double) *b, (double) *dt,
         (double) *begin_cut, (double) *end_cut,
         *cuterr, out, nout);
}
void
cut__(float *y, int *npts, float *b, float *dt,
      float *begin_cut, float *end_cut, int *cuterr,
      float *out, int *nout) {
    cutd(y, *npts, (double) *b, (double) *dt,
         (double) *begin_cut, (double) *end_cut,
         *cuterr, out, nout);
}
