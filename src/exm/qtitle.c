/** 
 * @file   qtitle.c
 * 
 * @brief  Report TITLE parameters
 * 
 */

#include "eam.h"
#include "gem.h"
#include "exm.h"

/** 
 * Report current values of the TITLE command 
 * 
 * @date   820316:  Original version.
 *
 */
void 
qtitle() {
  replv( "TITLE option$",14, cmgem.title.on );
  repkv( "Text of title$",15, kmgem.ktitl,145 );
  reprv( "SIZE of title$",15, cmgem.title.text_size );
  repav( "LOCATION of title$",19, (char*)kmgem.ksides[cmgem.title.pos - 1],9 );
  return;
}

