
#include "gdm.h"

/** 
 * Get current viewspace attributes
 *
 * @param lfull
 *    Full or fixed aspect ratio
 *    - TRUE for full viewspace option
 *    - FALSE for fixed aspect ratio option
 * @param ratio
 *    Current fixed aspect ratio of \p lfull is FALSE
 *    Unused if \p lfull is TRUE
 *
 * @date   890927:  Original version.
 *
 */
void 
getvspacetype(int *lfull,
              float *ratio)
{
	/* - Return current variables from common. */
	*lfull = cmgdm.lvsful;
	*ratio = cmgdm.vsrat;

}

