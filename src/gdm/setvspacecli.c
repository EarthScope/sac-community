
#include "gdm.h"

/** 
 * Turn Viewspace clipping on or off
 *
 * @param lclip
 *    - TRUE to turn viewspace clipping on
 *    - FALSE to turn viewspace clipping off
 *
 * @date   870501:  Original version.
 *
 */
void 
setvspaceclip(int lclip)
{
	/* - Save input variables in common. */
	cmgdm.lvsclip = lclip;
}

