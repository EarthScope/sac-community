/** 
 * @file   qcut.c
 * 
 * @brief  Report cut parameters
 * 
 */

#include "eam.h"
#include "dfm.h"


#include "exm.h"

/** 
 * Report the current values of the cut parameters
 * 
 * @date   920325:  Bug fix by Guy Tytgat, Alaska. Change kmut to kcut.
 * @date   870728:  Original version based on qcut.
 *
 */
void 
qcut() {
  float tmp[2];
  tmp[0] = cmdfm.ocut[0];
  tmp[1] = cmdfm.ocut[1];
  reprtw( "CUT option$",12, cmdfm.lcut, (char*)kmdfm.kcut,9, tmp );
  return;
}

