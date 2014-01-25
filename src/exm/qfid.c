/** 
 * @file   qfid.c
 * 
 * @brief  Report Fileid parameters
 * 
 */

#include "eam.h"
#include "gam.h"


#include "exm.h"

/** 
 * Report the current values of the fileid parameters
 * 
 * @date   820921:  Original version.
 *
 */
void
qfid() {
  replv( "FILEID display$",16, cmgam.lfidrq );
  repav( "Fileid display TYPE$",21, (char*)kmgam.kfidtp[cmgam.ifidtp - 1],9 );
  repav( "Fileid display LOCATION$",25, (char*)kmgam.kfidlc[cmgam.ifidlc - 1],9 );
  return;
}

