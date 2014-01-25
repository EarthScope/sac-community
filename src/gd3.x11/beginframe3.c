/*******************************************************************************
** PURPOSE:
*    To begin a frame to the XWindow device.
*
** OUTPUT ARGUMENTS:
*    nerr:  Error flag.  Set to 0 if no error occurs. (Pointer)
*
** SUBROUTINES CALLED:
*    erase3_
*******************************************************************************/

#include "gd3.x11.h"

void
beginframe3(nerr)
  int *nerr;
{
  *nerr = 0;
  
  /* Erase the window */
  erase3();

  /* Set the Current Text Size */
  settextsize3( cmgdm.twidth, cmgdm.thgt );

}

/*******************************************************************************
** MODIFICATION HISTORY:
*    920320:  Portability to IBM.
*    890607:  Modified to run under X11 rather than X10.  (kjm)
*    870227:  Original Version
*******************************************************************************/
