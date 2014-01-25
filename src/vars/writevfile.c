/** 
 * @file   writevfile.c
 * 
 * @brief  Write vars list to disk
 * 
 */

#include <stdio.h>
#include <string.h>

#include "amf.h"
#include "bool.h"
#include "vars.h"
#include "msg.h"
#include "co.h"
#include "bbf.h"
#include "debug.h"

/** 
 * Write a vars list in memory to disk
 * 
 * @param vars 
 *    Name of vars list 
 * @param vars_s 
 *    Length of \p vars
 * @param file 
 *    Name of file to write to.  Set to blanks if filename
 *    is to be the same as vars name
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *    - VARSLISTNOTFOUND (1203) Variable List not Found
 *
 * @note Local Variables
 *   - lopen:      Flag to know whether file has been opened or not. [l]
 *   - nun:        File unit number file is open on. [i]
 *   - jvars:      Index to requested vars list. [i]
 *   - numwords:   Number of words to copy from memory to disk file. [i]
 *   - nlocdisk:   Location (zero based) in disk file to start write. [i]   
 *   - index:      Index in sacmem array to start write. [i]
 *
 * @date   890227:  Changed logic regarding file name and vars name.
 * @date   870916:  Original version.
 * @date   870916:  Documented/Reviewed
 *
 */
void 
writevfile(char *vars, 
           int   vars_s, 
           char *file,
           int  *nerr) {
  UNUSED(vars_s);
  if((*nerr = sac_vars_write(vars, file))) {
    error(*nerr, "%s", file);
  }
  return;
}

