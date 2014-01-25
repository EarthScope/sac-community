/** 
 * @file   qline.c
 * 
 * @brief  Report line parameters
 * 
 */

#include "eam.h"
#include "gem.h"


#include "exm.h"

/** 
 * Report current values of the line parameters
 * 
 * @date   910301:  Changed iline to icline. iline was in gem and gam. (wct)
 * @date   870728:  Original version.
 *
 */
void 
qline() {

  replv( "LINE option$",13, cmgem.lline );
  if( cmgem.lline ){
    repiv( "Current linestyle$",19, cmgem.icline );
    replv( "Line INCREMENT option$",23, cmgem.liline );
    if( cmgem.liline )
      repivl( "Line increment LIST$",21, cmgem.iiline, cmgem.niline );
  }
  
  return;
}

