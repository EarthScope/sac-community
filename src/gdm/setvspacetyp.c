
#include "gdm.h"

/** 
 * Set the Viewspace type
 *  
 * @param lfull
 *    - TRUE for full viewspace option
 *    - FALSE for fixed aspect ratio option
 * @param ratio
 *    Desired fixed ratio if \p lfull is FALSE
 *    Ignored if \p lfull is TRUE
 *  
 * @date   861021:  Original version.
 *
 */
void 
setvspacetype(int lfull,
              double ratio)
{
	/* - Save input variables in common. */
	cmgdm.lvsful = lfull;
	cmgdm.vsrat = ratio;

}

