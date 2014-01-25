/** 
 * @file   inibbs.c
 * 
 * @brief  Initialize the "Blackboard function"
 * 
 */

#include <string.h>

#include "bbs.h"

/** 
 * Initialize the "Blackboard function" Common Blocks
 * 
 * @date  870301:  Original version.
 * @date  870301:  Documented / Reviewed
 *
 */
void 
inibbs() {

  strcpy( kmbbs.knmbbs, "blackboard");
  cmbbs.nlnbbs = 128;
  strcpy( kmbbs.kbbsinit, "INITDONE" );
  
  return;
}

