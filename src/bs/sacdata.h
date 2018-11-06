
#ifndef _SACDATA_H_
#define _SACDATA_H_

#include <SacHeader.h>

typedef struct sac_hdr_t sac_hdr;
struct sac_hdr_t {

};

#define CASSERT(predicate, file) _impl_CASSERT_LINE(predicate,__LINE__,file)

#define _impl_PASTE(a,b) a##b
#define _impl_CASSERT_LINE(predicate, line, file) \
  typedef char _impl_PASTE(assertion_failed_##file##_line ,line)[2*!!(predicate)-1];

CASSERT(sizeof(struct SACheader) == 656, SacHeader_h)

typedef struct _sacmeta sacmeta;
struct _sacmeta {
    int swap;
    char *filename;
    int data_read;
    int nstop;
    int nstart;
    int nfillb;
    int nfille;
    int ntotal;
};

typedef struct _sac sac;
struct _sac {
    struct SACheader *h;
    int n;
    float *y;
    float *x;
    sacmeta *m;
    int *sddhdr;           /* SDD Header - Length MWESHD - 164 */
};

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

#endif /* _SACDATA_H_ */
