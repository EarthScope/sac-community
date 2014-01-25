/** 
 * @file   vflist.c
 * 
 * @brief  Verify files are in the data file list
 * 
 */

#include "dfm.h"

#include "errors.h"


#include "msg.h"

/** 
 * Verify that there is at least one file in the data file list
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_NO_DATA_FILES_READ_IN
 *
 * @date   820622:  Original version.
 *
 */
void 
vflist(int *nerr) {

  *nerr = 0;
  
  if( cmdfm.ndfl <= 0 ){
    *nerr = ERROR_NO_DATA_FILES_READ_IN;
    setmsg( "ERROR", *nerr );
  }
  return;
}

