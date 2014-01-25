/** 
 * @file   gettextwait.c
 * 
 * @brief  Get Current text output wait mode 
 * 
 */

#include <string.h>

#include "exm.h"
#include "co.h"

/** 
 * Get the current text output wait mode 
 * 
 * @param mode 
 *    - "ON" causes sac to pause and wait for the user to respond
 *             after a screen full of output has been generated
 *    - "OFF" causes sac to not pause
 * @param mode_s 
 *    Length of \p mode
 *
 * @bug Change \p mode to an int / enum
 *
 * @date   900410:  Original version.
 *
 */
void 
gettextwait (char *mode, 
             int mode_s) {
  fstrncpy( mode, mode_s-1, kmexm.ktextwait, strlen(kmexm.ktextwait));
  return;
}

