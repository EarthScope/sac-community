/** 
 * @file   qylabl.c
 * 
 * @brief  Report YLABEL parameters
 * 
 */

#include "eam.h"
#include "gem.h"
#include "exm.h"

/** 
 * Report current values of the ylable command parameters
 * 
 * @date   820316:  Original version.
 *
 */
void 
qylabl() {

  replv( "YLABEL option$",15, cmgem.ylabel.on );
  repkv( "Text of ylabel$",16, kmgem.kylab,145 );
  reprv( "SIZE of ylabel$",16, cmgem.ylabel.text_size );
  repav( "LOCATION of ylabel$",20, (char*)kmgem.ksides[cmgem.ylabel.pos - 1], 9 );
  return;
}

