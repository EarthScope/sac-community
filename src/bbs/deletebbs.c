/** 
 * @file   deletebbs.c
 * 
 * @brief  Delete the "Blackboard Store"
 * 
 */

#include "bbs.h"
#include "vars.h"

/** 
 * Delete the "Blackboard Store"
 * 
 * @param nerr 
 *   - Error Return Flag
 *   - 0 on Success
 *   - Non-Zero on Failure
 *
 * @date    880412:  Original version.
 * @date    880412:  Documented / Reviewed
 */
void 
deletebbs(int *nerr) {

  deletevlist( kmbbs.knmbbs,MCPFN+1, "MEMORY", nerr );

  return;
} 

