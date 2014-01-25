/** 
 * @file   cerr.c
 * 
 * @brief  Format and store a parsing error
 * 
 */

#include "com.h"
#include "cpf.h"

#include "errors.h"


#include "msg.h"

/** 
 * Format and store a command parsing error
 * 
 * @param nerr 
 *    Error number
 *
 * @date   821004:  Removed error message for error number 1001.
 * @date   820415:  Original version.
 */
void 
cerr(int nerr) {

  cmcom.ncerr = nerr;
  
  if( cmcom.ncerr != ERROR_BAD_COMMAND_SYNTAX ){
    setmsg( "ERROR", cmcom.ncerr );
    //apimsg( cmcom.jcom );
  }
  
  return;
}

