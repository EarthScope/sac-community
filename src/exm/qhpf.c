/** 
 * @file   qhpf.c
 * 
 * @brief  Report HYPO parameters
 * 
 */

#include "eam.h"


#include "exm.h"

/** 
 * Report current values of the HYPO pick file parameters
 * 
 * @date   870728:  Original version.
 *
 */
void
qhpf() {
  repav( "HYPO pick file$",16, kmeam.khpfnm, MCPFN+1 );
  return;
}


