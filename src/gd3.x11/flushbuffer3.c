/** 
 * @file flushbuffer3.c
 *
 */

#include "gd3.x11.h"

#include "config.h"

/** 
 * Flush the Graphics Buffer
 * 
 * @param *nerr;
 *    Error Return Code
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date   890608:  Modified to run under X11 rather than X10.  (kjm)
 * @date   870223:  Original Version
 *
 */
void 
flushbuffer3(int *nerr)
{
  *nerr = 0;
#ifdef USE_X11_DOUBLE_BUFFER
  expose3();
#endif	    
}

