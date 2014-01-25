
#ifndef _AMF_H_
#define _AMF_H_


/** 
 * @struct t_cmmem
 *    Memory Storage for Header and Data Blocks
 *
 */

#include "debug.h"

struct t_cmmem {
  int     nallocated;  /** Number of allocated Blocks */
  float **sacmem;      /** Allocated Blocks, List of Pointers */
};

#ifdef DOINITS
  struct t_cmmem cmmem = { 0, NULL };
#else
  extern struct t_cmmem cmmem;
#endif

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
void relamb ( float **array, 
              int index, 
              int *nerr);

#endif  /* _AMF_H_ */
