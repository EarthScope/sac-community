
#ifndef _AMF_H_
#define _AMF_H_

#include <sacio/sacio.h>

#include "strlist.h"
#include "errors.h"

void iniam();

sac *sacget(int i, int data, int *nerr);
void sacput(sac * s);
void sacpop();
void sacdel(int i);
int saclen();
void sacclear();
void sacsort(int (*compare) (const void *a, const void *b));
sac *sacget_current();
int sacget_current_id();
void sac_header_copy(sac * to, sac * from);

int sac_find_filename(char *file);

sac *bflget(string_list * list, int i);
void bflclear();

sac *sac_new();
void sac_free(sac * s);
void sac_alloc(sac * s);
int sac_comps(sac * s);
void sac_extrema(sac * s);

char *khdr(sac * s, int k);

void sac_buffer_free();
sac *sac_copy(sac *s);
void sac_meta_copy(sac *to, sac *from);
void sac_be(sac *s);

#define CALC_E(s) (s->h->b + s->h->delta * (float)(s->h->npts - 1))

#define FHDR(s) ((float*) (&s->h->delta))
#define NHDR(s) ((int*) (&s->h->nzyear))
#define IHDR(s) ((int*) (&s->h->iftype))
#define LHDR(s) ((int*) (&s->h->leven))
#define TN(s)   ((float*) (&s->h->t0))
#define KHDR(s) ((char *) (&s->h->kstnm))

#define VALUE(x) (*(x))

#ifdef WIN32
#define STATIC_INLINE static __inline
#else
#define STATIC_INLINE static inline
#endif

STATIC_INLINE float *
fhdr(sac * s, int i) {
    float *fp = (float *) (&(s->h->delta));
    return &(fp[i - 1]);
}

STATIC_INLINE int *
ihdr(sac * s, int i) {
    int *ip = (int *) (&s->h->iftype);
    return &(ip[i - 1]);
}

STATIC_INLINE int *
nhdr(sac * s, int i) {
    int *ip = (int *) (&s->h->nzyear);
    return &(ip[i - 1]);
}

STATIC_INLINE int *
lhdr(sac * s, int i) {
    int *ip = (int *) (&s->h->leven);
    return &(ip[i - 1]);
}


#endif /* _AMF_H_ */
