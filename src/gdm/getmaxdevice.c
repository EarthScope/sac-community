
#include "gdm.h"

/** 
 * Get the maximum number of graphics devices
 *
 * @param number
 *    Maximum number of graphics devices in library
 *    This is the maximum static number of devices
 *
 * @date   870514:  Original version.
 *
 */
void 
getmaxdevices(int *number)
{
	*number = gdm_get_ndevices();
}

