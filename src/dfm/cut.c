
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
cut(float *in, int nstart, int nstop, int nfillb, int nfille, float *out) {
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

/* From defcut - only acts on times, not picks */
void
cut_define(float b, float delta, double dt, int *n) {
    int iTime, iBegin;

    iBegin = lround((double)b / (double)delta);

    /* Compute time and index */
    iTime = lround((double)dt / (double)delta);
    *n = iTime - iBegin + 1;

}

void
cut_define_check(float start, float stop, int npts, int cuterr, int *nstart,
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
cut_(float *in, int *nstart, int *nstop, int *nfillb, int *nfille, float *out) {
    cut(in, *nstart, *nstop, *nfillb, *nfille, out);
}

void
cut__(float *in, int *nstart, int *nstop, int *nfillb, int *nfille, float *out) {
    cut(in, *nstart, *nstop, *nfillb, *nfille, out);
}

void
cut_define_(float *b, float *delta, float *dt, int *n) {
    cut_define(*b, *delta, *dt, n);
}

void
cut_define__(float *b, float *delta, float *dt, int *n) {
    cut_define(*b, *delta, *dt, n);
}

void
cut_define_check_(float *start, float *stop, int *npts, int *cuterr,
                  int *nstart, int *nstop, int *nfillb, int *nfille,
                  int *nerr) {
    cut_define_check(*start, *stop, *npts, *cuterr, nstart, nstop, nfillb,
                     nfille, nerr);
}

void
cut_define_check__(float *start, float *stop, int *npts, int *cuterr,
                   int *nstart, int *nstop, int *nfillb, int *nfille,
                   int *nerr) {
    cut_define_check(*start, *stop, *npts, *cuterr, nstart, nstop, nfillb,
                     nfille, nerr);
}
