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
static buffer* sac_buffer = NULL;

buffer *buffer_new();
void buffer_set_format(buffer *b, char f);
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
 * \see     iniam allamb relamb reaamb
 *
 * \date   940127:  Original version.
 * \date   070606: Documented/Reviewed
 */
void 
iniam(struct t_cmmem *memstruct)
{
        int i;

        sac_buffer_new();
        return ;
        if ( memstruct->nallocated != 0 ){
	  /* not the first time in.  
	     release previously allocated memory and reinitialize 
	  */

          for (i=0; i<memstruct->nallocated; i++){
                  free(memstruct->sacmem[i]);
                  memstruct->sacmem[i] = NULL;
		}
	}
        else {
	  /* first time in. 
	     initialize the sacmem storage area.  
	     allocate block of pointers 
	  */
          if((memstruct->sacmem =(float **)malloc(MEMINIT*sizeof(float *))) == NULL){
            printf("Error allocating initial memory-iniam\n  quitting\n");
            exit(1);
	  }
          memstruct->nallocated = MEMINIT;
          for (i=0; i<MEMINIT; i++){
            memstruct->sacmem[i] = NULL;
	  }
	}

	return;
}

void
sacmem_free(struct t_cmmem *mem) {
  int i;
  if(mem->nallocated > 0) {
    for(i = 0; i < mem->nallocated; i++) {
      FREE(mem->sacmem[i]);
    }
  }
  FREE(mem->sacmem);
}


int
buffer_check(buffer *b) {
  if(!b) {
    fprintf(stderr, "buffer: check failed\n");
    return 0;
  }
  return 1;
}

void
buffer_init(buffer *b) {
  if(!buffer_check(b)) {
    return;
  }
  b->buf        = NULL;
  b->len        = 0;
  b->alloc      = 0;
  b->format     = 0;
}

buffer *
buffer_new() {
  buffer *b;

  b = (buffer *) malloc(sizeof(buffer));
  if(!b) {
    return NULL;
  }
  buffer_init(b);
  return b;
}

void
buffer_free_data(buffer *b) {
  if(!buffer_check(b)) {
    return;
  }
  if(b->buf) {
    free(b->buf);
    b->buf = NULL;
  }
  b->len = 0;
  b->alloc = 0;
}
void *
buffer_get(buffer *b, int i) {
  return b->buf[i];
}

void *
buffer_del(buffer *b, int i) {
  int j;
  void **p;
  if(i < 0 || i >= b->len) {
    printf("buffer: attempt to delete non-existant value");
    return NULL;
  }
  switch(b->format)
  case 'p': {
    p = b->buf[i];
    for(j = i; j < b->len-1; j++) {
      b->buf[j] = b->buf[j+1];
    }
  }
  b->len -= 1;
  return p;
}

void *
buffer_pop(buffer *b) {
  return buffer_del(b, b->len-1);
}

int
buffer_grow(buffer *b, int len) {
  void *tmp;
  if(b->len + len >= b->alloc) {
    if(b->alloc <= 0) {
      b->alloc = 1;
    }
    while((b->len + len) >= b->alloc) {
      b->alloc = b->alloc * 2;
    }
    tmp = (void *) realloc(b->buf, b->alloc * b->itemsize);
    if(!tmp) {
      fprintf(stderr, "buffer: reallocating error\n");
      return 0;
    }
    b->buf = tmp;
  }
  return 1;
}

/** 
 * Set the format of a buffer
 *
 * @param b
 *   Buffer Object
 * @param f
 *   - "i" Integer
 *   - "u" Unsigned Integer
 *   - "f" Floating point
 *   - "d" Double percision 
 *   - "s" string
 *   - "p" Pointer
 *
 * If the format of a buffer already exists, the data is freed
 *
 */
void
buffer_set_format(buffer *b, char f) {
  if(!buffer_check(b)) {
    return;
  }
  if(b->format) {
    buffer_free_data(b);
  }
  b->format = f;
  switch(b->format) {
  case 'i':
    b->itemsize = sizeof(int);
    break;
  case 'u':
    b->itemsize = sizeof(unsigned int);
    break;
  case 'f':
    b->itemsize = sizeof(float);
    break;
  case 'd':
    b->itemsize = sizeof(double);
    break;
  case 's':
    b->itemsize = sizeof(char *);
    break;
  case 'p':
    b->itemsize = sizeof(void *);
    break;
  default:
    break;
  }
  buffer_grow(b, 4);
}

void
buffer_sort(buffer *b, int (* compare)(const void *a, const void *b)) {
  qsort(b->buf, b->len, b->itemsize, compare);
}

void
buffer_append(buffer *b, void *p, int n) {
  if(!buffer_check(b)) {
    return;
  }
  if(!buffer_grow(b, n)) {
    return;
  }
  switch(b->format) {
  case 'p':{
    void ** pin = (void **) p;
    void ** pa  = &(b->buf[b->len]);
    int i;
    for(i = 0; i < n; i++) {
      pa[i] = pin[i];
    }
  }
    break;
  }
  //memcpy(b->buf + (b->len * b->itemsize), p, n * b->itemsize);
  b->len = b->len + n;
}


void
buffer_free(buffer *b) {
  if(!buffer_check(b)) {
    return;
  }
  buffer_free_data(b);
  if(b) {
    free(b);
    b = NULL;
  }
}

void
sac_hdr_init(struct SACheader *sh) {
  if(sh) {
    memcpy(sh, &NullSacHeader, sizeof(NullSacHeader));
    sh->nvhdr  = SAC_HEADER_MAJOR_VERSION;

    sh->leven  = TRUE;
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
  if(sh) {
    sac_hdr_init(sh);
  }
  return sh;
}

sacmeta *
sac_meta_new() {
  sacmeta *m;
  m = (sacmeta *) malloc(sizeof(sacmeta));
  if(m) {
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
sac_free(sac *s) {
  if(s) {
    FREE(s->h);
    FREE(s->x);
    FREE(s->y);
    if(s->m) {
      FREE(s->m->filename);
    }
    FREE(s->m);
    FREE(s->sddhdr);
    FREE(s);
  }
}

int
sac_data_exists(sac *s) {
  return s->m->data_read;
}

sac *
sac_new() {
  sac *s;
  s = (sac *) malloc(sizeof(sac));
  if(s) {
    s->h = sac_hdr_new();
    if(!s->h) {
      goto ERROR;
    }
    s->m = sac_meta_new();
    if(!s->m) {
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
  if(!sac_buffer) {
    sac_buffer_new();
  }
  if(i < 0 || i >= sac_buffer->len) {
    *nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
    return NULL;
  }
  s = buffer_get(sac_buffer, i);
  if(!s) {
    *nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
    return NULL;
  }
  if(data && !s->m->data_read) {
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
sacput(sac *s) {
  if(!sac_buffer) {
    sac_buffer_new();
  }
  if(!s->m->filename) {
    printf("Storing data without a filename\n");
  }
  buffer_append(sac_buffer, &s, 1);
  CURRENT_ID = saclen() - 1;
  CURRENT = s;
}

int
sac_comps(sac *s) {
  int n = 0;
  switch(s->h->iftype) {
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
sacsort(int (* compare)(const void *a, const void *b)) {
  buffer_sort(sac_buffer, compare);
}

void
sac_alloc(sac *s) {
  if(!s) {
    return;
  }
  FREE(s->y);
  FREE(s->x);
  s->y = (float *) malloc(sizeof(float) * s->h->npts);
  memset(s->y, 0, sizeof(s->h->npts) * sizeof(float));
  if( sac_comps(s) == 2 ) {
    s->x = (float *) malloc(sizeof(float) * s->h->npts);
    memset(s->x, 0, sizeof(s->h->npts) * sizeof(float));
  }
}

int
saclen() {
  return sac_buffer->len;
}

void
sacpop_no_free() {
  buffer_pop(sac_buffer);
}

void
sacdel(int i) {
  sac *s;
  s = buffer_del(sac_buffer, i);
  if(s) {
    if(s == CURRENT) {
      CURRENT = NULL;
      CURRENT_ID = -1;
    }
    sac_free(s);
    s = NULL;
  }
}

void
sacpop() {
  sacdel(sac_buffer->len - 1);
}

void
sacclear() {
  if(!sac_buffer) {
    sac_buffer_new();
  }
  while(saclen() > 0) {
    sacpop();
  }
}


void
sac_buffer_new() {
  if(! sac_buffer) {
    sac_buffer = buffer_new();
    buffer_set_format(sac_buffer, 'p');
  }
}

char *
khdr(sac *s, int k) {
  char *p;
  switch( k ){
  case 1: p = s->h->kstnm; break;
  case 2:
  case 3: p = s->h->kevnm; break;
  case 4: p = s->h->khole; break;
  case 5: p = s->h->ko; break;
  case 6: p = s->h->ka; break;
  case 7: p = s->h->kt0; break;
  case 8: p = s->h->kt1; break;
  case 9: p = s->h->kt2; break;
  case 10: p = s->h->kt3; break;
  case 11: p = s->h->kt4; break;
  case 12: p = s->h->kt5; break;
  case 13: p = s->h->kt6; break;
  case 14: p = s->h->kt7; break;
  case 15: p = s->h->kt8; break;
  case 16: p = s->h->kt9; break;
  case 17: p = s->h->kf; break;
  case 18: p = s->h->kuser0; break;
  case 19: p = s->h->kuser1; break;
  case 20: p = s->h->kuser2; break;
  case 21: p = s->h->kcmpnm; break;
  case 22: p = s->h->knetwk; break;
  case 23: p = s->h->kdatrd; break;
  case 24: p = s->h->kinst; break;
  default: p = NULL; break;
  }
  return p;
}

void
sac_extrema(sac *s) {
  extrma( s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax, &s->h->depmen );
}

int 
sac_find_filename(char *file) {
  int i, nerr;
  sac *s;
  for(i = 0; i < saclen(); i++) {
    if(!(s = sacget(i, TRUE, &nerr))) {
      return -1;
    }
    if(s->m->filename &&
       strncmp(s->m->filename, file, strlen(s->m->filename)) == 0) {
      return i;
    }
  }
  return -1;
}
