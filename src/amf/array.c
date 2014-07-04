
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#ifdef WIN32
#define pointer char *
#else
#define pointer void *
#endif

struct xarray_header {
  size_t len;
  size_t nalloc;
  char   type;
  size_t size;
  char   buf[];
};

struct xarray_header *
xarray_header(void *a) {
  struct xarray_header *ah;
  if(!a || !(ah = (struct xarray_header *)((pointer) a - offsetof(struct xarray_header, buf)))) {
    return NULL;
  }
  return ah;
}

int
xarray_length(void *a) {
  struct xarray_header *ah;
  if(!(ah = xarray_header(a))) {
    return 0;
  }
  return ah->len;
}

void *
xarray_new_with_len(char t, size_t n) {
  struct xarray_header *ah;
  size_t size;
  size_t nalloc = 2;
  switch(t) {
  case 'p': size = sizeof(void *); break;
  case 'i': size = sizeof(int); break;
  case 'f': size = sizeof(float); break;
  case 'd': size = sizeof(double); break;
  case 'c': size = sizeof(char); break;
  default: size = 0; break;
  }
  if(size == 0) {
    printf("arr invalid element type\n");
    return NULL;
  }
  while(n > nalloc) {
    nalloc *= 2;
  }
  ah = malloc(sizeof *ah + size * nalloc);
  if(!ah) {
    return NULL;
  }
  ah->len    = n;
  ah->nalloc = nalloc;
  ah->type   = t;
  ah->size   = size;
  return (void *) ah->buf;
}

void *
xarray_new(char t) {
  return xarray_new_with_len(t, 0);
}
  

void
xarray_free(void *a) {
  struct xarray_header *ah;
  if(!(ah = xarray_header(a))) {
    return;
  }
  free(ah);
}

void
xarray_clear(void *a) {
  struct xarray_header *ah;
  if(!(ah = xarray_header(a))) {
    return;
  }
  memset(a, 0, ah->size * ah->nalloc);
  ah->len = 0;
}

struct xarray_header *
xarray_grow(struct xarray_header *ah, size_t n) {
  struct xarray_header *new;
  if(n < ah->nalloc) {
    return ah;
  }
  while(n >= ah->nalloc) {
    ah->nalloc *= 2;
  }
  new = realloc(ah, sizeof(struct xarray_header) + ah->size * ah->nalloc );
  if(!new) {
    return NULL;
  }
  return new;
}

void *
xarray_append(void *a, ...) {
  struct xarray_header *ah;
  va_list ap;
  if(!(ah = xarray_header(a))) {
    return NULL;
  }
  ah = xarray_grow(ah, ah->len + 1);
  va_start(ap, a);
  switch(ah->type) {
  case 'p': ((void **) (ah->buf))[ah->len] =         va_arg(ap, void *); break;
  case 'i': ((int *)   (ah->buf))[ah->len] =         va_arg(ap, int);    break;
  case 'f': ((float *) (ah->buf))[ah->len] = (float) va_arg(ap, double); break;
  case 'd': ((double *)(ah->buf))[ah->len] =         va_arg(ap, double); break;
  case 'c': ((char *)  (ah->buf))[ah->len] = (char)  va_arg(ap, int);    break;
  default: break;
  }
  va_end(ap);
  ah->len++;
  return ah->buf;
}

void
xarray_delete(void *a, int i) {
  int n;
  struct xarray_header *ah;
  if(!(ah = xarray_header(a))) {
    return;
  }
  if(i < 0 || (size_t)i > ah->len) {
    printf("xarray: attempt to delete a non-existant value\n");
    return;
  }
  n = ah->len - 1 - i;
  memmove((pointer) a + ah->size * i, (pointer)a + ah->size * (i+1), ah->size * n);
  ah->len--;
}

void
xarray_pop(void *a) {
  struct xarray_header *ah;
  if(!(ah = xarray_header(a))) {
    return;
  }
  xarray_delete(a, ah->len - 1);
}

void
xarray_sort(void *a, int (*compare)(const void *a, const void *b)) {
  struct xarray_header *ah;
  if(!(ah = xarray_header(a))) {
    return;
  }
  qsort(a, ah->len, ah->size, compare);
}

