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
#include "clf.h"
#include "debug.h"
#include "SacHeader.h"
#include "hdr.h"
#include "ucf.h"
#include "scm.h"
#include "array.h"
#include "dbh.h"

static sac **sac_buffer = NULL;

void sac_buffer_new();

/** \def MEMINIT
 *  Initial number of blocks allocated 
 */
#define MEMINIT 100

/** 
 * A Memory Manager initializer.
 * 
 * If the memory block has not been allocated, 
 * then the Memory structure is allocated with a value of MEMINIT
 *
 * If the memory block has alredy been allocated with some value, Not Zero, 
 * then All elements of the Memory structure is deallocted 
 * 
 * \param *memstruct
 *    Memory structure begin initialized or free'd
 *
 * \return  Nothing
 * \see     iniam 
 *
 * \date   940127:  Original version.
 * \date   070606: Documented/Reviewed
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
sac_hdr_init(struct SACheader *sh) {
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

struct SACheader *
sac_hdr_new() {
    struct SACheader *sh;
    sh = (struct SACheader *) malloc(sizeof(struct SACheader));
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
    memmove(to->h, from->h, sizeof(struct SACheader));
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

void
sac_extrema(sac * s) {
    extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax, &s->h->depmen);
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
    float junk;
    if(s->h->leven) {
        s->h->e = calc_e_even(s);
    } else {
        if(s->x) {
            extrma(s->x, 1, s->h->npts, &s->h->b, &s->h->e, &junk);
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

