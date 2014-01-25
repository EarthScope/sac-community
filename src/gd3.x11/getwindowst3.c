/*******************************************************************************
** PURPOSE:
*    To get the status of a window.
*
** INPUT ARGUMENTS:
*    win_num:  Window number to get status of. (Pointer)
*
** OUTPUT ARGUMENTS:
*    exists:  Status of window.  Set to 1 if window is available for plotting.
*
** GLOBAL INPUT:
*    gd3.x11.h:  plotw3->status.
*******************************************************************************/

#include "gd3.x11.h"

void
getwindowstat3(win_num, exists)
  int win_num, *exists;
{

/* Get status of window */

  if (basew3[win_num].status == AVAILABLE)
    *exists = 1;
  else
    *exists = 0;
 
}

/*******************************************************************************
** MODIFICATION HISTORY:
*    890608:  Used as-is from X10 version.  (kjm)
*    870318:  Changes due to gd3.x10.h structure change.
*    870223:  Original Version
*******************************************************************************/
