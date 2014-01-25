/** 
 * @file   begingraphic.c
 * 
 * @brief  Graphic Initilization Routine
 * 
 */

#include <stdio.h>
#include <math.h>

#include "gpm.h"

#include "errors.h"


#include "gtm.h"
#include "gdm.h"

/** 
 * To begin of initialize the SAC Graphics Library
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   861027:  Original version.
 *
 */
void 
begingraphics(int *nerr) {

  *nerr = SAC_OK;
  
  /* - Initialize the Graphics Tool Module. */
  inigtm();
  
  /* - Initialize the Graphics Device Module. */
  inigdm( nerr );
  
  return;
}

