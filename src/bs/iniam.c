/**
 * @file   iniam.c
 *
 * @brief  Initialize the Array Manager
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include <sacio/sacio.h>
#include <fern/array.h>

#include "amf.h"
#include "hdr.h"

#include "debug.h"
#include "errors.h"
#include "strlist.h"

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

sac **
sac_data() {
    return sac_buffer;
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

int
sac_data_exists(sac * s) {
    return s->m->data_read;
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
void
sacsort(int (*compare) (const void *a, const void *b)) {
    xarray_sort(sac_buffer, compare);
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



#ifdef __TESTING__


// gcc -o meta_test meta.c -D__TESTING__ -I/usr/include/libxml2 -I../../ -I../../inc/ -lxml2 libfern.a /Users/savage13/Bits/sac/sac.build/102.u/libmseed/libmseed.a /Users/savage13/Bits/sac/sac.build/102.u/src/libsacio.a libtime.a -lcurl

int
main() {
    char out[256] = {0};
    int nerr = 0;
    sac *s = sac_read("XE.DOOR..BHZ.M.1994.160.003345.sac", &nerr);

    sac_fmt(out, sizeof(out), "%N %S %L %C %TB %TE", s);
    printf("%s\n", out);
    sac_fmt(out, sizeof(out), "%Z", s);
    printf("%s\n", out);
    sac_fmt(out, sizeof(out), "%R", s);
    printf("%s\n", out);
    sac_fmt(out, sizeof(out), "%tb %te", s);
    printf("%s\n", out);
    char *fmt =  "//s:Network[@code='%N']/s:Station[@code='%S']"
        "/s:Channel[@locationCode='%H' and @code='%C']";
    sac_fmt(out, sizeof(out), fmt, s);
    printf("%s\n", out);

}

#endif
