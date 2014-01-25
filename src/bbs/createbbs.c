/** 
 * @file   createbbs.c
 * 
 * @brief  Create the "Blackboard Store"
 * 
 */

#include "bbs.h"
#include "vars.h"

/** 
 * Create the "Blackboard Store"
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - Non-Zero on Failure
 *
 * @date    870301:  Original version.
 * @date    870301:  Documented / Reviewed
 */
void 
createbbs(int *nerr) {
  int notused;

  
  sac_vars_delete( kmbbs.knmbbs );
  createvlist( kmbbs.knmbbs,MCPFN+1, cmbbs.nlnbbs, &notused, nerr );
  
  return;
} 

