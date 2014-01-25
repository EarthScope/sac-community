/** 
 * @file   qxlim.c
 * 
 * @brief  Report XLIM parameters
 * 
 */

#include "eam.h"
#include "gam.h"


#include "exm.h"

/** 
 * Report current values of the xlim parameters
 * 
 * @date   830121:  Original version.
 *
 */
void 
qxlim() {
  float tmp[2];
  tmp[0] = cmgam.ortwxl[0];
  tmp[1] = cmgam.ortwxl[1];
  reprtw( "XLIM option$",13, cmgam.lrtwxl, (char*)kmgam.krtwxl,9, tmp);
  return;
}

