/*******************************************************************************
** PURPOSE:
*    To get the geometry of the SGF device.
*
** INPUT ARGUMENTS:
*  
*  
*
** GLOBAL INPUT:
*    gd2.h:  XW, YW
*              
*
** GLOBAL OUTPUT:
*  
*
** SUBROUTINES CALLED:
*  
*
** LOCAL VARIABLES:
*  
*******************************************************************************/

#include "gd2.h"
#include "debug.h"

void get_geometry2( number, width_return, height_return, nerr)
     int number;
     unsigned int *width_return, *height_return;
     int *nerr;
{
  UNUSED(number);
    *nerr = 0;
    *width_return = (int)XW;
    *height_return =(int)YW;

}

/*******************************************************************************
** MODIFICATION HISTORY:
*
*
*    940823:  Original Version
*******************************************************************************/
