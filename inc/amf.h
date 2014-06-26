
#ifndef _AMF_H_
#define _AMF_H_


/** 
 * @struct t_cmmem
 *    Memory Storage for Header and Data Blocks
 *
 */

#include "debug.h"
#include "SacHeader.h"
#include "clf.h"
#include "errors.h"

struct t_cmmem {
  int     nallocated;  /** Number of allocated Blocks */
  float **sacmem;      /** Allocated Blocks, List of Pointers */
};

/*
#ifdef DOINITS
  struct t_cmmem cmmem = { 0, NULL };
#else
  extern struct t_cmmem cmmem;
#endif
*/
void allamb ( struct t_cmmem *memstruct, 
              int nsize, 
              int *index, 
              int *nerr);
void iniam  ( struct t_cmmem *memstruct);
void reaamb ( float **array, 
              int nsize, 
              int nosize, 
              int index, 
              int *newndx, 
              int *nerr);

typedef struct _buffer buffer;
struct _buffer {
  void  **buf;
  int     len;
  int     alloc;
};

sac *sacget(int i, int data, int *nerr);
void sacput(sac* s);
void sacpop();
void sacdel(int i);
int  saclen();
void sacclear();
void sacsort(int (*compare)(const void *a, const void *b));
sac *sacget_current();
int sacget_current_id();
void sac_header_copy(sac *to, sac *from);

int sac_find_filename(char *file);

sac *bflget(string_list *list, int i);
void bflclear();

sac *sac_new();
void sac_free(sac *s);
void sac_alloc(sac *s);
int sac_comps(sac *s);
void sac_extrema(sac *s);

char *khdr(sac *s, int k);

#endif  /* _AMF_H_ */
