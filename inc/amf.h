
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
xfhdr(sac * s, int i) {
    float *fp = (float *) (&(s->h->_delta));
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

STATIC_INLINE double
sac_float(sac *s, int id) {
    double v = 0.0;
    sac_get_float(s, id, &v);
    return v;
}

#define   A(s) sac_float(s, SAC_A)
#define   F(s) sac_float(s, SAC_F)
#define   E(s) sac_float(s, SAC_E)
#define   O(s) sac_float(s, SAC_O)
#define   B(s) sac_float(s, SAC_B)
#define  SB(s) sac_float(s, SAC_SB)
#define  T0(s) sac_float(s, SAC_T0)
#define  DT(s) sac_float(s, SAC_DELTA)
#define SDT(s) sac_float(s, SAC_SDELTA)

#define EVLO(s) sac_float(s, SAC_EVLO)
#define EVLA(s) sac_float(s, SAC_EVLA)
#define EVDP(s) sac_float(s, SAC_EVDP)
#define EVEL(s) sac_float(s, SAC_EVEL)
#define STLO(s) sac_float(s, SAC_STLO)
#define STLA(s) sac_float(s, SAC_STLA)
#define STEL(s) sac_float(s, SAC_STEL)
#define STDP(s) sac_float(s, SAC_STDP)

#endif /* _AMF_H_ */
