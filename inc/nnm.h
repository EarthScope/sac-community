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
    char kwritenn[MCPFN + 1];
} ;

/** 
 * @struct cmnnm 
 *    Neural Net Module Variables
 *
 */
struct t_cmnnm {
    int numpoints;
    int numfiles;
} ;

#define NNM_EXTERN \
    extern struct t_kmnnm kmnnm; \
    extern struct t_cmnnm cmnnm;

void xnnmc(int index, int *nerr);
void xwritenn(int *nerr);

#endif /* _NNM_H_ */
