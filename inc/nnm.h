/** 
 * @file   nnm.h
 * 
 * @brief  Neural Net Module
 * 
 */

#ifndef _NNM_H_
#define _NNM_H_

#include "mach.h"

/** 
 * @struct kmnnm 
 *    Neural Net Module Characters
 *
 */
struct t_kmnnm {
  char kwritenn[MCPFN+1];
} kmnnm;

/** 
 * @struct cmnnm 
 *    Neural Net Module Variables
 *
 */
struct t_cmnnm {
  int   numpoints;
  int   numfiles;
  float headerarray[MDFL];
} cmnnm;


#ifdef DOINITS

   float *const Headerarray = &cmnnm.headerarray[0] - 1;

#else

   extern float *const Headerarray;

#endif

void xnnmc ( int index, 
             int *nerr);
void xwritenn ( int *nerr);

#endif /* _NNM_H_ */
