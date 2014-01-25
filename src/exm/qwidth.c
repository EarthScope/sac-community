/** 
 * @file   qwidth.c
 * 
 * @brief  Report WIDTH parameters
 * 
 */

#include "eam.h"
#include "gem.h"


#include "exm.h"

/** 
 * Report current values of the WIDTH parameters
 * 
 * @date   920528:  Original version - copied from qcolor and modified.
 *
 */
void 
qwidth() {

  replv( "WIDTH option$",14, cmgem.lwidth );
  if( cmgem.lwidth ){
    replv( "WIDTH INCREMENT option$",24, cmgem.liwidth );
    repiv( "Current WIDTH value$",21, cmgem.iwidth );
    repiv( "Current SKELETON WIDTH value$",30, cmgem.iskwidth );
    if( cmgem.liwidth )
      repivl( "WIDTH increment LIST:$",23, cmgem.iiwidth, cmgem.niwidth );
  }
  return;
}

