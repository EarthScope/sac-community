/** 
 * @file   rsac1.c
 * 
 * @brief  Read an evenly spaced SAC file
 * 
 */
#include "config.h"

#include <string.h>
#include <sys/types.h>
#include "unistdx.h"

#ifndef WIN32
#include <sys/uio.h>
#endif

#include "amf.h"
#include "dff.h"
#include "bool.h"
#include "hdr.h"
#include "msg.h"
#include "co.h"
#include "ucf.h"
#include "SacHeader.h"
#include "errors.h"
#include "defs.h"

#include <fstr.h>

void
sac_data_swap(float *y, int n) {
    int i;
    float *ptr;
    for (i = 0, ptr = y; i < n; i++, ptr++) {
        byteswap((void *) ptr, SAC_HEADER_SIZEOF_NUMBER);
    }
}

/** 
 * Read in sac data from file \p nun into \p yarray of length \p npts an swap
 *   if necessary
 * 
 * @param nun 
 *    Logical file unit retured from zopen_sac
 * @param yarray 
 *    Output array where data is to be stored
 * @param npts 
 *    Length of array \p yarray
 @ @param comp
 *    Which Component to read
 * @param lswap 
 *    Swap the data if true
 * @param nerr 
 *    Error Return Flag
 *    - SAC_OK on Success
 *    - Non-Zero on Error
 *
 * @date July 01, 2008 - Initial Version  -- B. Savage
 *
 */
void
sac_data_read(int nun, float *yarray, int npts, int comp, int lswap, int *nerr) {
    UNUSED(comp);
    *nerr = SAC_OK;

    if (read(-nun, yarray, npts * SAC_DATA_SIZE) != (npts * SAC_DATA_SIZE)) {
        *nerr = ERROR_READING_FILE;
        return;
    }
    if (lswap) {
        sac_data_swap(yarray, npts);
    }
}

/** 
 * Read an evenly spaced SAC file
 * 
 * @param kname 
 *    Name of disk file to read
 * @param yarray 
 *    Output data from the disk file
 * @param nlen 
 *    Number of data points to read, should be less than or equal to \p max_
 * @param beg 
 *    Beginning Time, B Header Value
 * @param del 
 *    Sampling Interval, Delta Header Value
 * @param max_ 
 *    Size of \o yarray
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *    - 801 if file is not evenly spaced
 *    - 803 if number of points in file is greater than \p max_
 *          In this case the first \p max points are read
 *    - 1317 Error Determining SAC file Type, Not a SAC file
 * @param kname_s 
 *    Length of \p kname
 *
 * @date   Aug  4 2008 B. Savage <savage_at_uri.edu>
 *                     Changed wrtmsg() to outmsg()
 * @date   Mar 18 2007 B. Savage <savage_at_uri.edu>
 *                     Updated routines for better interpability with C
 *                     and Fortran when used as an external library
 * @date   870902:  Added calls to INILHF and INIMSG as part of initialization.
 * @date   870513:  Changed call to wrtxtd to wrtmsg.
 * @date   830125:  Changes due to modified header common block.
 * @date   810212:  Changed to output message retrieval from disk.
 * @date   800919:  Original version.
 * @date   870902:  Documented/Reviewed
 *
 */
void
rsac1(char *kname, float *yarray, int *nlen, float *beg, float *del, int *max_,
      int *nerr, int kname_s) {
    int ncerr, nun;
    int lswap;
    int truncated;
    sac *s;

    *nerr = 0;
    truncated = FALSE;
    nun = 0;

    /* - Initialize some common blocks if not already done. */
    sacio_initialize_common();

    /* - Open the file. */
    zopen_sac(&nun, kname, kname_s, "RODATA", 7, nerr);
    if (*nerr != SAC_OK)
        goto ERROR;

    s = sac_new();
    s->m->filename = fstrdup(kname, kname_s);
    sacput(s);

    //lswap = sac_header_read(nun, nerr);
    lswap = sac_header_read(nun, s, nerr);
    if (*nerr != SAC_OK)
        goto ERROR;

    /* - Make sure file is evenly spaced. */
    if (s->h->leven) {
        if (s->h->npts <= *max_) {
            *nlen = s->h->npts;
        } else {
            *nlen = *max_;
            truncated = TRUE;
        }
        *beg = s->h->b;
        *del = s->h->delta;
    } else {
        *nerr = ERROR_SAC_FILE_NOT_EVENLY_SPACED;
        error(*nerr, "%s", s->m->filename);
        goto ERROR;
    }

    /* - Read in the data. */
    sac_data_read(nun, yarray, *nlen, SAC_FIRST_COMPONENT, lswap, (int *) nerr);
    if (nerr != SAC_OK) {
        error(*nerr, "%s", s->m->filename);
        goto ERROR;
    }
    s->y = yarray;
    /* - Adjust several header fields. */
    s->h->npts = *nlen;
    sac_be(s);

  ERROR:
    *nerr = (*nerr == SAC_OK &&
             truncated == TRUE) ? -ERROR_SAC_DATA_TRUNCATED_ON_READ : *nerr;
    if (*nerr) {
        outmsg();
        clrmsg();
    }
    zclose(&nun, &ncerr);
    return;
}

void
rsac1_(char *kname, float *yarray, int *nlen, float *beg, float *del, int *max_,
       int *nerr, int kname_s) {
    rsac1(kname, yarray, nlen, beg, del, max_, nerr, kname_s);
}

void
rsac1__(char *kname, float *yarray, int *nlen, float *beg, float *del,
        int *max_, int *nerr, int kname_s) {
    rsac1(kname, yarray, nlen, beg, del, max_, nerr, kname_s);
}

int
sac_data_read_new(sac *s, FILE *fp) {
    float *p;
    int i;
    size_t n;
    n = s->h->npts;
    for(i = 0; i < sac_comps(s); i++) {
        p = (i == 0) ? s->y : s->x ;
        if(fread(p, sizeof(float), n, fp) != n) {
            return ERROR_READING_FILE;
        }
        if(s->m->swap) {
            sac_data_swap(p, s->h->npts);
        }
    }
    return 0;
}


double
check_precision(float dt, float val) {
    float df = val - nextafterf(val,0.0);
    if(df > dt) {         // Return difference if larger than sampling rate
        return df;
    }
    return 0.0;           // Otherwise return 0.0
}

void
sac_check_time_precision(sac_hdr *h) {
    int i, n;
    double df;
    char *names[] = {"b","e","a","o","t0","t1","t2","t3","t4","t5","t6","t7","t8","t9","f"};
    float values[] = {h->b, h->e, h->a, h->o,
                      h->t0,h->t1,h->t2,h->t3,h->t4, h->t5,h->t6,h->t7,h->t8,h->t9,
                      h->f};
    n = sizeof(values)/sizeof(float);
    for(i = 0; i < n; i++) {
        if(values[i] == SAC_FLOAT_UNDEFINED) {
            continue;
        }
        if((df = check_precision(h->delta, values[i])) != 0) {
            warning(3264, "minimum precision > sampling rate: %s = %f\n"
                    "       sampling rate (delta):      %f\n"
                    "       32-bit minimum precision:   %f",
                    names[i], values[i], h->delta, df, values[i]);
            outmsg();
            clrmsg();
        }
    }
}

int
sac_header_read_new(sac *s, FILE *fp) {
    int nerr;
    size_t n;
    char str[SAC_HEADER_STRINGS * 8];
    //fprintf(stderr, "sac hdr: %p\n", s->h);
    n = SAC_HEADER_NUMBERS;
    if(fread((char *) s->h, sizeof(float), n, fp) != n) {
        return ERROR_READING_FILE;
    }
    s->m->swap = sac_check_header_version((float *) s->h, &nerr);
    if(nerr) {
        return ERROR_READING_FILE;
    }
    if(s->m->swap) {
        sac_header_swap((float *) s->h);
    }

    if(fread(str, 8, SAC_HEADER_STRINGS, fp) != SAC_HEADER_STRINGS) {
        return ERROR_READING_FILE;
    }
    map_chdr_in((float *) s->h->kstnm, (float *) str);

    sac_check_time_precision(s->h);

    return 0;
}

sac *
sac_read_internal(char *filename, int read_data, int *nerr) {
    FILE *fp;
    sac *s;

    *nerr = 0;
    s = NULL;
    //fprintf(stderr, "filename: %s\n", filename);
    sacio_initialize_common();

    if(!filename) {
        return NULL;
    }

    if(!(fp = fopen(filename, "rb"))) {
        *nerr = 101;
        return NULL;
    }
    s = sac_new();

    s->m->filename = strdup(filename);
    *nerr = sac_header_read_new(s, fp);
    if(s->h->iftype <= 0) {
        exit(-1);
    }

    if(*nerr) {
        goto ERROR;
    }
    if(read_data) {
        //fprintf(stderr, "alloc: %d\n", s->h->npts);
        sac_alloc(s);
        s->m->nstart = 1;
        s->m->nstop  = s->h->npts;
        s->m->ntotal = s->h->npts;
        s->m->nfillb = 0;
        s->m->nfille = 0;
        if((*nerr = sac_data_read_new(s, fp))) {
            goto ERROR;
        }
    }
    sac_be(s);
    update_distaz(s);
    if(read_data) {
        sac_extrema(s);
    }

    fclose(fp);
    return s;

 ERROR:
    if(s) {
        sac_free(s);
        s = NULL;
    }
    fclose(fp);
    return NULL;
}

sac *
sac_read_header(char *filename, int *nerr) {
    return sac_read_internal(filename, 0, nerr);
}

sac *
sac_read(char *filename, int *nerr) {
    return sac_read_internal(filename, 1, nerr);
}

