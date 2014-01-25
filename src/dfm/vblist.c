/** 
 * @file   vblist.c
 * 
 * @brief  Verify a binop file 
 * 
 */

#include "dfm.h"
#include "bom.h"

#include "errors.h"


#include "msg.h"

/** 
 * Verify that there is at least one file in the binop file list
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_BINOP_FILE_LIST_EMPTY
 *
 * @date   820721:  Documented subroutine.
 * @date   820721:  Original version.
 *
 */
void 
vblist(int *nerr) {
  *nerr = 0;
  
  if( cmbom.nbfl <= 0 ){
    *nerr = ERROR_BINOP_FILE_LIST_EMPTY;
    setmsg( "ERROR", *nerr );
  }
  
  return;
}

