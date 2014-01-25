
#include "gdm.h"

/** 
 * Get the current line style
 *
 * @param istyle
 *   Current linestyle attribute
 *   - 1 - Solid Line
 *   - > 1 device specific line style
 *
 * @date   861026:  Original version.
 *
 */
void 
getlinestyle(int *istyle)
{
	*istyle = cmgdm.iline;

}

