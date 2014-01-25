/*******************************************************************************
** PURPOSE:
*    To change the auxiliary colors in the color table.
*
** INPUT ARGUMENTS:
*    icolortable: Requested auxiliary color table.  
** GLOBAL OUTPUT:
*
*
** SUBROUTINES CALLED:
*    XGetWindowAttributes,XAllocColorCells,XStoreColors
*
** LOCAL VARIABLES:
*    Status:  Status of color cells.
*    i:       Loop counter.
*    planes:  Plane mask.
*    pixels:  Pixel values.
*    full:    'Fullest color' -- Colors are in range [0, full].
*
** LIMITATIONS:
*    - Maximum of 256 colors.
*******************************************************************************
** MODIFICATION HISTORY:
*    940921:  Original Version
*******************************************************************************/

#include "gd2.h"
#include "gem.h"
#include "gdm.h"
#include "debug.h"

#include "sgfcolor.h"

#define MGREY 1
#define MCOLOR 2

void
changectable2(nentry,icolortable)
  int nentry, icolortable;
{ 

    int i;
    UNUSED(nentry);
    for (i = 0; i < cmgdm.npscimage; i++){
      if(icolortable == MCOLOR){
      sred[i] = sgfred[i];
      sgreen[i] = sgfgreen[i];
      sblue[i] = sgfblue[i];
      } else {
      sred[i] = 255-i;
      sgreen[i] = 255-i;
      sblue[i] = 255-i;
      }
    }
}

