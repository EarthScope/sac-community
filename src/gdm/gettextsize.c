
#include <stdio.h>

#include "gdm.h"

/** 
 * Get current graphics text size
 *
 * @param width
 *    Width of a single character in viewport units
 * @param height
 *    Height of a single character in viewport units
 *
 * @date   831026:  Original version.
 *
 */
void 
gettextsize(float *width,
            float *height)
{

	*width = cmgdm.twidth;
	*height = cmgdm.thgt;

}

