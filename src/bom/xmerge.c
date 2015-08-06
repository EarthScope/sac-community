/** 
 * @file   xmerge.c
 * 
 * @brief  Execute MERGE
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amf.h"
#include "bool.h"
#include "bom.h"
#include "dfm.h"
#include "hdr.h"
#include "cpf.h"
#include "msg.h"
#include "co.h"
#include "dff.h"
#include "ucf.h"
#include "bot.h"
#include "errors.h"
#include "clf.h"
#include "ssi.h"

//#define __DEBUG__
#include "debug.h"

BOM_EXTERN

static int verbose_merge = FALSE;

/** 
 * Execute the MERGE command.  This command merges two sets of data files.
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *
 * @date   980928:  Enforce a commit or rollback prior to executing merge.
 *             maf.
 * @date   980605:  Allow files that overlap to be merged. maf
 * @date   970822:  Added eexact to fix subtle bug in timing.  It gets the
 *             value of ennd1 + delta rounded to the millisecond.  maf
 * @date   920110:  Added DATA-SET update logic.
 * @date   900119:  Added CHECKTIMES option.
 * @date   870811:  Added logic to fill gap with zeros if necessary.
 *             Also made it an error if there is an overlap.
 * @date   850617:  Major rewrite due to addition of new memory manager.
 * @date   840206:  Fixed bug in writing scratch file headers to disk.
 * @date   820331:  Combined "parse" and "control" modules.
 * @date   810224:  Changed BFL storage location.
 * @date   810130:  Original version.
 *
 */

enum {
    GAP_FILL_ZERO = 1,
    GAP_FILL_INTERPOLATE = 2,
};
enum {
    OVERLAP_AVERAGE = 1,
    OVERLAP_COMPARE = 2,
};

struct timing {
    long int bsec;
    double psec;
    int i;
    float dt;
    int npts;
    int bn;
    int en;
    float *y;
    int alloc;
    double offset;              /* partial sample offset */
};

int
is_leap_year(int y) {
    if (y % 400 == 0) {
        return 1;
    }
    if (y % 100 == 0) {
        return 0;
    }
    if (y % 4 == 0) {
        return 1;
    }
    return 0;
}

static int days_in_year[] =
    { 0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

int
days_before_month(int y, int m) {
    int d;
    d = days_in_year[m];
    if (m > 2 && is_leap_year(y)) {
        d++;
    }
    return d;
}

void
doy2ymd(int y, int doy, int *m, int *d) {
    int dbm;
    *m = 1;
    dbm = days_before_month(y, *m);
    while (dbm < doy) {
        (*m)++;
        dbm = days_before_month(y, *m);
    }
    *m -= 1;
    if (*m <= 0) {
        *m = 1;
    }
    *d = doy - days_before_month(y, *m);
}

#define DAYS_PER_YEAR 365L
#define MINS_PER_HOUR 60L
#define SECS_PER_MIN  60L
#define HOURS_PER_DAY 24L
#define SECS_PER_HOUR (SECS_PER_MIN * MINS_PER_HOUR)
#define SECS_PER_DAY (SECS_PER_HOUR * HOURS_PER_DAY)

int
days_before_year(int y) {
    return (y - 1) * DAYS_PER_YEAR + (y - 1) / 4 - (y - 1) / 100 + (y -
                                                                    1) / 400;
}

int
ymd2doy(int y, int m, int d) {
    return days_before_month(y, m) + d;
}

int
ymd2ord(int y, int m, int d) {
    return days_before_year(y) + ymd2doy(y, m, d);
}

long int
hms2s(int h, int m, int s) {
    return h * SECS_PER_HOUR + m * SECS_PER_MIN + s;
}

long int
ymdhms2s(int y, int m, int d, int h, int min, int s) {
    long int sec;
    sec = (ymd2ord(y, m, d) - 1L) * SECS_PER_DAY;
    return sec + hms2s(h, min, s);
}

long int
secs_in_AD(sac * s) {
    int m, d;
    doy2ymd(s->h->nzyear, s->h->nzjday, &m, &d);
    return ymdhms2s(s->h->nzyear, m, d, s->h->nzhour, s->h->nzmin, s->h->nzsec);
}

int
timing_cmp(const void *pa, const void *pb) {
    struct timing *a = (struct timing *) pa;
    struct timing *b = (struct timing *) pb;
    if (a->bsec == b->bsec) {   /* seconds equal, partial seconds */
        if (a->psec < b->psec) {
            return -1;
        }
        if (a->psec > b->psec) {
            return 1;
        }
    } else {                    /* seconds differ */
        if (a->bsec < b->bsec) {
            return -1;
        }
        if (a->bsec > b->bsec) {
            return 1;
        }
    }
    return 0;
}

//static int
sac *
get_file(string_list * list, int i, int *y) {
    int nerr;
    UNUSED(y);
    if (i <= saclen()) {
        return sacget(i - 1, TRUE, &nerr);
    } else {
        return bflget(list, i - saclen() - 1);
    }
    return NULL;                //nerr;
}

int
vkstnm(string_list * list) {
    int i, y, n;
    char stn[9], net[9], cmp[9];
    sac *s;
    n = saclen() + string_list_length(list);

    if (!(s = get_file(list, 1, &y))) {
        return -1;
    }
    strncpy(stn, s->h->kstnm, 8);
    stn[8] = 0;
    strncpy(net, s->h->knetwk, 8);
    net[8] = 0;
    strncpy(cmp, s->h->kcmpnm, 8);
    cmp[8] = 0;
    for (i = 2; i <= n; i++) {
        if (!(s = get_file(list, i, &y))) {
            return -1;
        }
        if (strcmp(stn, s->h->kstnm) != 0) {
            error(1801, "Station Name [KSTNM]: '%s' '%s'", stn, s->h->kstnm);
            return 1801;
        }
        if (strcmp(net, s->h->knetwk) != 0) {
            error(1801, "Network Name [KNETWK]: '%s' '%s'", net, s->h->knetwk);
            return 1801;
        }
        if (strcmp(cmp, s->h->kcmpnm) != 0) {
            error(1801, "Component Name [KCMPNM]: '%s' '%s'", cmp,
                  s->h->kcmpnm);
            return 1801;
        }
    }
    return 0;
}

struct timing *
time_range(string_list * list) {
    int i, n;
    double b;
    sac *s;

    struct timing *t;
    n = saclen() + string_list_length(list);
    DEBUG("create timing\n");
    t = (struct timing *) malloc(sizeof(struct timing) * n);
    for (i = 1; i <= n; i++) {
        DEBUG("%d/%d\n", i, n);
        if (!(s = get_file(list, i, NULL))) {
            fprintf(stderr, "Error getting file number: %d\n", i);
            return NULL;
        }
        t[i - 1].bsec = secs_in_AD(s) + (long int) floor(s->h->b);
        t[i - 1].psec = s->h->b - floor(s->h->b) + (s->h->nzmsec / 1000.0);
        if (t[i - 1].psec >= 1.0) {
            t[i - 1].bsec += (long int) floor(t[i - 1].psec);
            t[i - 1].psec -= floor(t[i - 1].psec);
        }
        if (t[i - 1].psec < 0.0) {
            t[i - 1].bsec -= (long int) floor(t[i - 1].psec);
            t[i - 1].psec += floor(t[i - 1].psec);
        }
        DEBUG("%d %ld %ld %f %f %d %d %f\n", i - 1, secs_in_AD(s),
              t[i - 1].bsec, t[i - 1].psec, s->h->b, s->h->nzmin, s->h->nzsec,
              s->h->t0);
        DEBUG("%d %d %d %d %d %d\n", s->h->nzyear, s->h->nzjday, s->h->nzhour,
              s->h->nzmin, s->h->nzsec, s->h->nzmsec);
        t[i - 1].i = i;
        t[i - 1].dt = s->h->delta;
        t[i - 1].npts = s->h->npts;
        t[i - 1].bn = 0;
        t[i - 1].en = 0;
        t[i - 1].alloc = FALSE;
        if (i <= saclen()) {
            t[i - 1].y = s->y;
        } else {
            t[i - 1].alloc = TRUE;
            t[i - 1].y = malloc(sizeof(float) * s->h->npts);
            memcpy(t[i - 1].y, s->y, sizeof(float) * s->h->npts);
        }
    }
    DEBUG("sort timing\n");
    qsort(t, n, sizeof(struct timing), timing_cmp);
    for (i = 0; i < n; i++) {
        b = (double) (t[i].bsec - t[0].bsec) + (t[i].psec - t[0].psec);
        t[i].offset = b / t[i].dt;
        t[i].bn = lround(t[i].offset);
        t[i].en = lround((t[i].npts - 1) + (b / t[i].dt));
        t[i].offset -= t[i].bn;
        DEBUG("%d: %d %d PTS %d %d\n", i, t[i].bn, t[i].en, t[i].npts,
              t[i].en - t[i].bn);
        if (fabs(t[i].offset) < 0.03) { /* Close to 0.0 => 0.0 */
            t[i].offset = 0.0;
        }
        if (t[i].offset < 0.0) {        /* All offset are positive */
            t[i].offset += 1.0;
        }
        if (t->offset > 0.0) {
            while (t[i].bn * t[i].dt < b) {
                /* Make sure the first point to interpolate on is between two known points  */
                t[i].bn++;
                t[i].en++;
            }
            while (t[i].en * t[i].dt > (b + (t[i].npts - 1) * t[i].dt)) {
                t[i].en--;
            }
        }
        DEBUG("offset: %5f b: %5f dt: %5f b,e: %d,%d (%d) [%d]\n", t[i].offset,
              b, t[i].dt, t[i].bn, t[i].en, t[i].npts, t->offset == 0.0);
    }
    return t;
}

int
check_delta(string_list * list) {
    int i, y, n;
    float dt;
    sac *s;
    n = saclen() + string_list_length(list);

    if (!(s = get_file(list, 1, &y))) {
        return FALSE;
    }
    dt = s->h->delta;
    for (i = 2; i <= n; i++) {
        if (!(s = get_file(list, i, &y))) {
            return FALSE;
        }
        if (fabs(dt - s->h->delta) > 1e-7) {
            error(1801, "Time Sampling [DELTA]: %f %f\n", dt, s->h->delta);
            return 1801;
        }
        if (!s->h->leven) {
            return 1306;
        }
    }
    return 0;
}

void
fill_zero(float y[], int b, int e, float dt) {
    int i;
    int err = TRUE;
    DEBUG("%d -> %d (%f %f)\n", b, e, b + (b * s->h->dt), b + (e * dt));
    for (i = b; i < e; i++) {
        if (verbose_merge) {
            printf("merge: Gap zero fill: [n: %d t: %f]\n", i, b + i * dt);
        } else if (err) {
            printf("merge: Gap zero fill\n");
            err = FALSE;
        }
        y[i] = 0.0;
    }
}

void
fill_interp(float y[], int b, int e, float yb, float ye, float dt) {
    int i;
    int err = TRUE;
    for (i = b; i < e; i++) {
        if (verbose_merge) {
            printf("merge: Gap interp fill: [n: %d t: %f]\n", i, b + i * dt);
        } else if (err) {
            printf("merge: Gap interp fill\n");
            err = FALSE;
        }
        y[i] = yb + i * (ye - yb) / (e - b);
    }
}

int *
files_in_window(int b, int e, struct timing *t, int n, int *mp) {
    int i, m, *ij;
    ij = (int *) malloc(sizeof(int) * n);
    m = 0;
    for (i = 0; i < n; i++) {
        if (t[i].en >= b && t[i].bn < e) {
            ij[m++] = i;
        }
    }
    *mp = m;
    return ij;
}

float
interp1(float y0, float y1, float dt) {
    /*   x0       x1        x2  -- time
     *   |   +--dt-+         |
     *   .....................
     *       |         |
     *      y0         y1       -- amplitude
     */
    return y0 + (y1 - y0) * dt;
}

float
tinterp(struct timing *t, int j) {
    if (t->offset > 0.0) {
        if (j == t->npts) {
            return interp1(t->y[j - 1], t->y[j - 2], 1.0 - t->offset);
        } else if (j + 1 < t->npts) {
            return interp1(t->y[j], t->y[j + 1], t->offset);
        }
        fprintf(stderr, "Array access attempt out of bounds: %d %d\n", j + 1,
                t->npts);
        return 0.0;
    }
    if (j == t->npts) {
        return t->y[t->npts - 1];
    }
    if (j < 0 || j > t->npts) {
        fprintf(stderr, "Array access attempt out of bounds: %d %d\n", j,
                t->npts);
        return 0.0;
    }
    return t->y[j];
}

int
in_array(struct timing *t, int i) {
    if (t->offset == 0.0) {
        return TRUE;
    }
    if (t->offset > 0.0 && i - t->bn + 1 < t->npts) {
        return TRUE;
    }
    return FALSE;
}

void
overlap_average(float *y, int b, int e, struct timing *t, int n) {
    int i, j, k, m;
    int *ij;
    //DEBUG("%d -> %d\n", b, e);
    ij = files_in_window(b, e, t, n, &m);
    i = 0;
    for (i = b; i < e; i++) {   /* Loop over points */
        y[i] = 0.0;
        n = 0;
        for (k = 0; k < m; k++) {       /* Loop over files in window */
            j = ij[k];
            if (i > t[j].bn && i < t[j].en && in_array(&t[j], i)) {
                y[i] += tinterp(&t[j], i - t[j].bn);
                n++;
            }
        }
        if (n == 0) {
            fprintf(stderr, "No files to average, error\n");
            return;
        }
        if (n > 0) {
            y[i] = y[i] / n;
        }
    }
    FREE(ij);
}

int
overlap_compare(float *y, int b, int e, struct timing *t, int nt,
                string_list * list, float bval) {
    int i, j, k, m, n;
    int *ij, *ip;
    float *p;
    int retval;
    sac *s;
    retval = TRUE;
    //DEBUG("%d -> %d\n", b, e);
    ij = files_in_window(b, e, t, nt, &m);
    i = 0;
    p = (float *) malloc(sizeof(float) * m);
    ip = (int *) malloc(sizeof(int) * m);
    for (i = b; i <= e; i++) {  /* Loop over points */
        n = 0;
        for (k = 0; k < m; k++) {       /* Loop over files in window */
            j = ij[k];
            if (i >= t[j].bn && i <= t[j].en && in_array(&t[j], i)) {
                p[n] = tinterp(&t[j], i - t[j].bn);
                ip[n] = j;
                n++;
            }
        }
        if (n == 0) {
            fprintf(stderr, "No files to compare, error\n");
            return FALSE;
        }
        if (n > 1) {
            int error = FALSE;
            for (j = 1; j < n; j++) {
                if (fabs(p[0] - p[j]) > 1e-7) {
                    error = TRUE;
                }
            }
            if (error) {
                retval = FALSE;
                if (verbose_merge) {
                    printf("merge: Amplitude mismatch: [n: %d t: %f]\n", i,
                           i * t[0].dt + bval);
                    for (j = 0; j < n; j++) {
                        k = ip[j];
                        s = get_file(list, t[k].i - 1, NULL);
                        printf
                            ("    %15.7e (%d/%d) %15s [File # %d] Interp: %s\n",
                             p[j], i - t[k].bn, t[k].npts, s->m->filename,
                             t[k].i, (t[k].offset == 0.0) ? "No" : "Yes");
                    }
                }
            }
        }
        y[i] = p[0];
    }
    FREE(p);
    FREE(ip);
    FREE(ij);
    return retval;
}

int
single_copy(float y[], int b, int e, struct timing *t) {
    int i;
    for (i = b; i < e; i++) {
        if (in_array(t, i)) {
            y[i] = tinterp(t, i - t->bn);
        }
    }
    return (t->offset > 0.0) ? e - 1 : e;
}

void
xmerge_new(int *nerr) {

    struct timing *t;
    int i, n;
    float *y;
    int b, e, mb;
    char gap_keys[2][9] = { "ZERO    ", "INTERP  " };
    char overlap_keys[2][9] = { "AVERAGE ", "COMPARE " };
    string_list *list;
    sac *s, *s2;
    static int gap_fill = GAP_FILL_ZERO;
    static int overlap = OVERLAP_COMPARE;

    *nerr = 0;
    t = NULL;
    y = NULL;
    list = NULL;
    verbose_merge = FALSE;

    DEBUG("option parsing\n");

    while (lcmore(nerr)) {

        if ((lckey("VERBOSE$", 9))) {
            verbose_merge = TRUE;
        } else if (lklist("GAP$", 5, (char *) gap_keys, 9, 2, &gap_fill)) {
        } else if (lklist("OVERLAP$", 5, (char *) overlap_keys, 9, 2, &overlap)) {
        }

        /* -- define new binop data file list. */
        else if ((list = lcdfl_wild())) {
            cmbom.ibflc = 0;
        }

        /* -- Bad syntax. */
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }

    if (*nerr != 0)
        goto ERROR;

    if (!list) {
        list = string_list_init();
    }

    /* CHECKING PHASE: */
    /* - Check for null data file list. */
    vflist(nerr);
    if (*nerr) {
        /* - Check for a null binop file list. */
        if (!list || string_list_length(list) <= 0) {
            *nerr = ERROR_BINOP_FILE_LIST_EMPTY;
            error(*nerr, "");
            goto ERROR;
        }
    }
    /* Check station, network, and component */
    if ((*nerr = vkstnm(list))) {
        goto ERROR;
    }
    /* Check delta and if evenly spaced */
    if ((*nerr = check_delta(list))) {
        goto ERROR;
    }

    /* EXECUTION PHASE: */
    /* - Commit or rollback data according to cmdfm.icomORroll */
    if (saclen() > 0) {
        alignFiles(nerr);
        if (*nerr) {
            goto ERROR;
        }
    }

    t = time_range(list);
    n = saclen() + string_list_length(list);
    if (verbose_merge) {
        printf("merging %d files => %d data points\n", n, t[n - 1].en + 1);
    }
    /* Allocate space for time series */
    y = (float *) malloc(sizeof(float) * (t[n - 1].en + 1));
    memset(y, 0, sizeof(float) * (t[n - 1].en + 1));
    b = t[0].bn;
    e = t[0].en;

    /*
       for(i = 0; i < n; i++) {
       DEBUG("b: %ld %f [%d -> %d] %f\n", t[i].bsec, t[i].psec, t[i].bn, t[i].en, t[i].offset);
       DEBUG("id %d %p (%d, %f)\n", t[i].i, t[i].y, t[i].npts, t[i].dt);
       DEBUG("\n");
       }
     */

    /* Grab file with earliest time sample */
    if (!(s = get_file(list, t[0].i, NULL))) {
        *nerr = 1301;
        goto ERROR;
    }
    /*
     * Fill new time series, working forward point by point
     * mb - begin point of next file
     * e  - end point of current file
     * b  - last point data that was written to
     */
    for (i = 0; i < n; i++) {
        DEBUG("inserting file: %d/%d (%p) %d %d(?)->%d\n", i, n, t[i].y,
              t[i].npts, t[i].bn, t[i].en);
        if (i >= n - 1) {       /* Final File  b ..(DATA).. e */
            single_copy(y, b, t[i].en + 1, &t[i]);
            continue;
        }

        mb = t[i + 1].bn;
        if (e <= mb) {          /* Gap :: b ..(DATA).. e ..(GAP).. mb */
            e = single_copy(y, b, e + 1, &t[i]);
            if (gap_fill == GAP_FILL_ZERO) {
                fill_zero(y, e, mb, t[0].dt);
            } else if (gap_fill == GAP_FILL_INTERPOLATE) {
                fill_interp(y, e, mb, tinterp(&t[i], t[i].npts),
                            tinterp(&t[i + 1], 0), t[0].dt);
            } else {
                fprintf(stderr, "Unknown gap filling mechanism\n");
                goto ERROR;
            }
            b = mb;
            e = t[i + 1].en;
        } else if (e > mb) {    /* Overlap :: b ..(DATA).. mb ..(OVERLAP).. e */
            DEBUG("overlap %d -> %d\n", b, mb);
            mb = single_copy(y, b, mb, &t[i]);
            if (overlap == OVERLAP_AVERAGE) {
                overlap_average(y, mb, e, t, n);
            } else if (overlap == OVERLAP_COMPARE) {
                DEBUG("   compare %d %d\n", mb, e);
                if (!overlap_compare(y, mb, e, t, n, list, s->h->b)) {
                    *nerr = 9005;
                    goto ERROR;
                }
            } else {
                fprintf(stderr, "Unknown overlap filling mechanism\n");
                goto ERROR;
            }
            b = e;
            e = t[i + 1].en;
        }
    }

    s2 = get_file(list, t[0].i, NULL);
    s = sac_new();
    memcpy(s->h, s2->h, sizeof(struct SACheader));

    /* Filename is the first file */
    s2 = get_file(list, 1, NULL);
    s->m->filename = strdup(s2->m->filename);

    sacclear();
    if (string_list_length(list) > 0) {
        bflclear();
    }

    sacput(s);
    FREE(s->y);

    s->h->npts = t[n - 1].en + 1;
    s->h->e = s->h->b + s->h->delta * (float) (s->h->npts - 1);
    s->y = y;
    sac_extrema(s);

    /* Clean up */
    if (t) {
        for (i = 0; i < n; i++) {
            if (t[i].alloc) {
                FREE(t[i].y);
            }
        }
        FREE(t);
    }

    setrng();
    sacToSeisMgr(FALSE, FALSE, TRUE, nerr);

    return;

  ERROR:

    /* Clean up */
    if (t) {
        for (i = 0; i < n; i++) {
            if (t[i].alloc) {
                FREE(t[i].y);
            }
        }
        FREE(t);
    }
    FREE(y);
    if (string_list_length(list) > 0) {
        bflclear();
    }

    return;
}

void
xmerge(int *nerr) {
    xmerge_new(nerr);
}
