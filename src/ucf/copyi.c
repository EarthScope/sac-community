/** 
 * @file   copyi.c
 * 
 * @brief  Copy an integer array
 * 
 */

#include <string.h>

#include "ucf.h"

/** 
 * Copy an integer array
 * 
 * @param isrce 
 *    Where to copy from
 * @param isink 
 *    Where to copy to
 * @param ncopy 
 *    Length of \p isrce and \p isink
 *
 * @date   March 14, 2009 - Converted to use memmove() which handles overlapping memory areas
 * @date   ??????:  Fixed bug involving computation of overlap.
 * @date   831005:  Made independent by adding call to ZMEMAD.
 * @date   830810:  Replaced with version based upon ZCOPY.
 * @date   800822:  Fixed bug in the logic governing forward/reverse copying.
 * @date   800103:  Original Prime version.
 *
 */
void 
copyi(int *isrce, 
      int *isink, 
      int  ncopy) {
  memmove(isink, isrce, ncopy * sizeof(int));
}

