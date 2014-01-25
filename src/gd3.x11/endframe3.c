/*******************************************************************************
** PURPOSE:
*    To end plotting to a frame.
*
** OUTPUT ARGUMENTS:
*    nerr:  Error flag.  Set to 0 if no error occurs. (Pointer)
*
** SUBROUTINES CALLED:
*    flushbuffer3
*******************************************************************************/

#include "gd3.x11.h"

void
endframe3(nerr)
  int *nerr;
{
  *nerr = 0;
  flushbuffer3(nerr);

}

/*******************************************************************************
** MODIFICATION HISTORY:
*    890608:  Used as-is from X10 version.  (kjm)
*    870223:  Original Version
*******************************************************************************/
