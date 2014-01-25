/*******************************************************************************
** PURPOSE:  To create a label consisting of a string and an integer.
*
** INPUT ARGUMENTS:
*    string:  String part of label.
*    num:     Integer part of label.
*
** OUTPUT ARGUMENTS:
*    label:  Label -- String followed by integer.
*
** SUBROUTINES CALLED:
*    sprintf, strcpy, strcat
*******************************************************************************/

#include <stdio.h>
#include <string.h>

#include "gd3.x11.h"

void
make_label3(string, num, label)
  char string[];
  int *num;
  char label[];
{
  char kwin_num[8];

/* Convert integer to integer and concatenate it to string */

  sprintf(kwin_num, "%d", *num);
  strcpy(label, string);
  strcat(label, kwin_num);

}

/*******************************************************************************
** MODIFICATION HISTORY:
*    870318:  Original Version
*******************************************************************************/
