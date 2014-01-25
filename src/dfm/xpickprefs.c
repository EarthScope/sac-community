/** 
 * @file   xpickprefs.c
 * 
 * @brief  Control use of pick preferences file
 * 
 */

#include "dfm.h"
#include "bool.h"

#include "errors.h"


#include "msg.h"
#include "cpf.h"

/** 
 * Control the use of the picks preferences file. By default, the 
 *    pickPreferences file is not used in reading picks from CSS data 
 *    nor in passing data from SeisMgr to the SAC data buffers.
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_PICK_PREFS_UNKNOWN_OPTION
 *
 * @date   000606:  Original version.  maf
 *
 */
void 
xpickprefs(int *nerr) {

  *nerr = 0;
  
  if( lcmore( nerr ) ) {
    if( lckeyExact( "ON#$", 5 ) ) {
      cmdfm.lpref = TRUE ;
    }
    
    else if( lckeyExact( "OFF#$", 6 ) ) {
      cmdfm.lpref = FALSE ;
    }
    
    else{
      *nerr = ERROR_PICK_PREFS_UNKNOWN_OPTION ;
      setmsg( "ERROR" , *nerr ) ;
      outmsg() ;
    }
  }
  else
    cmdfm.lpref = !cmdfm.lpref ;
}
