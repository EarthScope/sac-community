/*******************************************************************************
** PURPOSE:
*    To get the device attributes.
*
** OUTPUT ARGUMENTS:
*    dev_name:         Name of the device.
*    dev_type:         Type of device. (Pointer)
*    dev_name_length:  Length of dev_name. (Pointer)
*
** GLOBAL INPUT:
*    gd3.x11.h:  device_name3, device_type3
*
** SUBROUTINES CALLED:
*    strcpy
*******************************************************************************/

#include <string.h>

#include "gd3.x11.h"
#include "debug.h"

void
getdeviceinfo3(dev_name, dev_name_length, dev_type)
  char dev_name[];
  int dev_name_length;
  int *dev_type;
{

/* Set attributes */
  UNUSED(dev_name_length);
  strcpy(dev_name, device_name3);
  *dev_type = device_type3;

}

/*******************************************************************************
** MODIFICATION HISTORY:
*    890608:  Used as-is from X10 version.  (kjm)
*    870223:  Original Version
*******************************************************************************/
