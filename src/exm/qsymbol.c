/** 
 * @file   qsymbol.c
 * 
 * @brief  Report SYMBOL parameters
 * 
 */

#include "eam.h"
#include "gem.h"


#include "exm.h"

/** 
 * Report the current values of the SYMBOL parameters
 * 
 * @date   870728:  Original version.
 *
 */
void 
qsymbol() {

  replv( "SYMBOL option$",15, cmgem.lsym );
  if( cmgem.lsym ){
    repiv( "Current SYMBOL$",16, cmgem.isym );
    reprv( "Symbol SIZE$",13, cmgem.symsz );
    reprv( "Symbol SPACEING$",17, cmgem.symsp );
    replv( "Symbol INCREMENT option$",25, cmgem.lisym );
    if( cmgem.lisym )
      repivl( "Symbol increment LIST$",23, cmgem.iisym, cmgem.nisym );
  }
  
  return;
}

