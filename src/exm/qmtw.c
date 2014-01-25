/** 
 * @file   qmtw.c
 *
 * @brief  Report MTW parameters
 * 
 */

#include "eam.h"
#include "smm.h"


#include "exm.h"

/** 
 * Report current values of the MTW parameters
 * 
 * @date   870728:  Original version based on qcut.
 *
 */
void 
qmtw() {
  char kmwt[9];
  float tmp[2];
  tmp[0] = cmsmm.omtw[0];
  tmp[1] = cmsmm.omtw[1];
  reprtw( "MTW option$",12, cmsmm.lmtw, kmwt,9, tmp );
  return;  
}

