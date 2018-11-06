/** 
 * @file   iniam.c
 * 
 * @brief  Initialize the Array Manager
 * 
 */
#include <stdio.h>
#include <stdlib.h>

#define SAC_NULL_HEADER_REQUIRED
#include "amf.h"
#include "hdr.h"

#include "debug.h"
#include "array.h"
#include "errors.h"
#include "strlist.h"
#include <sacdata.h>

static sac **sac_buffer = NULL;

void sac_buffer_new();

/**
 * Initialize the Memory Manager
 *
 */
void
iniam() {
    sac_buffer_new();
    return;
}

/*
  + new()
  + init()
  + grow()
  + put() => append()
  + extend() => append()
  + delete()
  + get()
  + clear() => free_data()
  + pop()
  + free()
  + length()

  - find()
  - print()

 */

void
sac_hdr_init(sac_hdr *sh) {
    if (sh) {
        memcpy(sh, &NullSacHeader, sizeof(NullSacHeader));
        sh->nvhdr = SAC_HEADER_MAJOR_VERSION;

        sh->leven = TRUE;
        sh->lpspol = FALSE;
        sh->lovrok = TRUE;
        sh->lcalda = TRUE;

        sh->iftype = ITIME;
    }
}

sac_hdr *
sac_hdr_new() {
    sac_hdr *sh;
    sh = (sac_hdr *) malloc(sizeof(sac_hdr));
    if (sh) {
        sac_hdr_init(sh);
    }
    return sh;
}

sacmeta *
sac_meta_new() {
    sacmeta *m;
    m = (sacmeta *) malloc(sizeof(sacmeta));
    if (m) {
        m->swap = FALSE;
        m->filename = NULL;
        m->data_read = TRUE;
        m->nstop = 0;
        m->nstart = 0;
        m->nfillb = 0;
        m->nfille = 0;
        m->ntotal = 0;
    }
    return m;
}

void
sac_free(sac * s) {
    if (s) {
        FREE(s->h);
        FREE(s->x);
        FREE(s->y);
        if (s->m) {
            FREE(s->m->filename);
        }
        FREE(s->m);
        FREE(s->sddhdr);
        FREE(s);
    }
}

int
sac_data_exists(sac * s) {
    return s->m->data_read;
}

sac *
sac_new() {
    sac *s;
    s = (sac *) malloc(sizeof(sac));
    if (s) {
        s->h = sac_hdr_new();
        if (!s->h) {
            goto ERROR;
        }
        s->m = sac_meta_new();
        if (!s->m) {
            goto ERROR;
        }
        s->n = 1;
        s->y = NULL;
        s->x = NULL;
        s->sddhdr = NULL;
    }

    return s;
  ERROR:
    sac_free(s);
    return NULL;
}

int CURRENT_ID = -1;
sac *CURRENT = NULL;

sac *
sacget(int i, int data, int *nerr) {
    sac *s;
    if (!sac_buffer) {
        sac_buffer_new();
    }
    if (i < 0 || (size_t) i >= xarray_length(sac_buffer)) {
        *nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
        return NULL;
    }
    s = sac_buffer[i];
    if (!s) {
        *nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
        return NULL;
    }
    if (data && !s->m->data_read) {
        *nerr = ERROR_ONLY_HEADERS_IN_MEMORY;
        return NULL;
    }
    *nerr = SAC_OK;
    CURRENT_ID = i;
    CURRENT = s;
    return CURRENT;
}

sac *
sacget_current() {
    return CURRENT;
}

int
sacget_current_id() {
    return CURRENT_ID;
}

void
sacput(sac * s) {
    if (!sac_buffer) {
        sac_buffer_new();
    }
    if (!s->m->filename) {
        printf("Storing data without a filename\n");
    }
    sac_buffer = xarray_append(sac_buffer, s);
    CURRENT_ID = saclen() - 1;
    CURRENT = s;
}

int
sac_comps(sac * s) {
    int n = 0;
    switch (s->h->iftype) {
        case ITIME:
        case IXY:
        case IUNKN:
            n = (s->h->leven) ? 1 : 2;
            break;
        case IXYZ:
            n = 1;
            break;
        case IRLIM:
        case IAMPH:
            n = 2;
            break;
        default:
            fprintf(stderr, "unknown sac data type: %d\n", s->h->iftype);
            n = 2;
            break;
    }
    return n;
}

void
sacsort(int (*compare) (const void *a, const void *b)) {
    xarray_sort(sac_buffer, compare);
}

void
sac_alloc(sac * s) {
    if (!s) {
        return;
    }
    FREE(s->y);
    FREE(s->x);
    s->y = (float *) malloc(sizeof(float) * s->h->npts);
    memset(s->y, 0, s->h->npts * sizeof(float));
    if (sac_comps(s) == 2) {
        s->x = (float *) malloc(sizeof(float) * s->h->npts);
        memset(s->x, 0, s->h->npts * sizeof(float));
    }
}

int
saclen() {
    return xarray_length(sac_buffer);
}

void
sacpop_no_free() {
    xarray_pop(sac_buffer);
}

void
sacdel(int i) {
    sac *s;
    s = sac_buffer[i];
    if (s) {
        if (s == CURRENT) {
            CURRENT = NULL;
            CURRENT_ID = -1;
        }
        sac_free(s);
        s = NULL;
    }
    xarray_delete(sac_buffer, i);
}

sac *
saclast() {
    int n = saclen();
    if(n > 0) {
        return sac_buffer[n-1];
    }
    return NULL;
}

void
sacpop() {
    sac *s = saclast();
    xarray_pop(sac_buffer);
    sac_free(s);
    s = NULL;
}

void
sacclear() {
    if (!sac_buffer) {
        sac_buffer_new();
    }
    while (saclen() > 0) {
        sacpop();
    }
}

void
sac_buffer_new() {
    if (!sac_buffer) {
        sac_buffer = xarray_new('p');
    }
}

void
sac_buffer_free() {
    sacclear();
    xarray_free(sac_buffer);
}

void
sac_header_copy(sac * to, sac * from) {
    memmove(to->h, from->h, sizeof(sac_hdr));
}
void
sac_meta_copy(sac *to, sac *from) {
    to->m->swap      = from->m->swap;
    to->m->filename  = strdup(from->m->filename);
    to->m->data_read = from->m->data_read;
    to->m->nstop     = from->m->nstop;
    to->m->nstart    = from->m->nstart;
    to->m->nfillb    = from->m->nfillb;
    to->m->nfille    = from->m->nfille;
    to->m->ntotal    = from->m->ntotal;
}
void
sac_data_copy(sac *to, sac *from) {
    if(from->y) {
        sac_alloc(to);
        memcpy(to->y, from->y, sizeof(float) * from->h->npts);
        if(sac_comps(to) == 2 && from->x) {
            memcpy(to->x, from->x, sizeof(float) * from->h->npts);
        }
    }
}

char *
khdr(sac * s, int k) {
    char *p;
    switch (k) {
        case 1:
            p = s->h->kstnm;
            break;
        case 2:
        case 3:
            p = s->h->kevnm;
            break;
        case 4:
            p = s->h->khole;
            break;
        case 5:
            p = s->h->ko;
            break;
        case 6:
            p = s->h->ka;
            break;
        case 7:
            p = s->h->kt0;
            break;
        case 8:
            p = s->h->kt1;
            break;
        case 9:
            p = s->h->kt2;
            break;
        case 10:
            p = s->h->kt3;
            break;
        case 11:
            p = s->h->kt4;
            break;
        case 12:
            p = s->h->kt5;
            break;
        case 13:
            p = s->h->kt6;
            break;
        case 14:
            p = s->h->kt7;
            break;
        case 15:
            p = s->h->kt8;
            break;
        case 16:
            p = s->h->kt9;
            break;
        case 17:
            p = s->h->kf;
            break;
        case 18:
            p = s->h->kuser0;
            break;
        case 19:
            p = s->h->kuser1;
            break;
        case 20:
            p = s->h->kuser2;
            break;
        case 21:
            p = s->h->kcmpnm;
            break;
        case 22:
            p = s->h->knetwk;
            break;
        case 23:
            p = s->h->kdatrd;
            break;
        case 24:
            p = s->h->kinst;
            break;
        default:
            p = NULL;
            break;
    }
    return p;
}

static float
array_min(float *y, int n) {
    float v = y[0];
    for(int i = 0; i < n; i++) {
        v = fmin(v, y[i]);
    }
    return v;
}
static float
array_max(float *y, int n) {
    float v = y[0];
    for(int i = 0; i < n; i++) {
        v = fmax(v, y[i]);
    }
    return v;
}
static float
array_mean(float *y, int n) {
    double v = 0.0;
    for(int i = 0; i < n; i++) {
        v += y[i];
    }
    return v / n;
}

static void
check_value(float vmin, float vmax) {
    if(vmin < -3.40282e38 || vmax > 3.40282e38) {
        printf(" WARNING: Data value outside system storage bounds\n");
        if(isinf(vmax)) {
            printf(" Maxvalue = %s ", (vmax < 0) ? "-inf":"inf");
        } else {
            printf(" Maxvalue = %-.5g ", vmax);
        }
        if(isinf(vmin)) {
            printf(" Minvalue = %s", (vmin < 0) ? "-inf":"inf");
        } else {
            printf(" Minvalue = %-.5g", vmin);
        }
        printf("\n");
    }
}

void
sac_extrema(sac * s) {
    s->h->depmin = array_min(s->y, s->h->npts);
    s->h->depmax = array_max(s->y, s->h->npts);
    s->h->depmen = array_mean(s->y, s->h->npts);
    check_value(s->h->depmin, s->h->depmax);
}

float
calc_e_even(sac *s) {
    switch (s->h->iftype) {
    case ITIME:
    case IXY:
    case IUNKN:
        return s->h->b + s->h->delta * (float)(s->h->npts - 1);
        break;
    case IRLIM:
    case IAMPH: {
        int nfreq = 0;
        if(s->h->npts % 2 == 0) {
            nfreq = s->h->npts / 2;
        } else {
            nfreq = (s->h->npts-1) / 2;
        }
        return s->h->b + (float) nfreq * s->h->delta;
    }
        break;
    case IXYZ:
        break;
    }
    return SAC_FLOAT_UNDEFINED;
}

void
sac_be(sac *s) {
    if(s->h->leven) {
        s->h->e = calc_e_even(s);
    } else {
        if(s->x) {
            s->h->b = array_min(s->x, s->h->npts);
            s->h->e = array_max(s->x, s->h->npts);
        }
    }
}


int
sac_find_filename(char *file) {
    int i, nerr;
    sac *s;
    for (i = 0; i < saclen(); i++) {
        if (!(s = sacget(i, TRUE, &nerr))) {
            return -1;
        }
        if (s->m->filename &&
            strncmp(s->m->filename, file, strlen(s->m->filename)) == 0) {
            return i;
        }
    }
    return -1;
}

sac *
sac_copy(sac *s) {
    sac *new;
    new = sac_new();
    sac_header_copy(new, s);
    sac_meta_copy(new, s);
    sac_data_copy(new, s);
    return new;
}

