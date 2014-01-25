#include <string.h>

#include "extfunc.h"
#include "debug.h"

void fgetahdr_(hdr_index, fieldname, value, error, lenfield, lenvalue)
int *hdr_index;
char *fieldname;
char     *value;
int     *error;
int   lenfield;
int   lenvalue;
{

  char infield[8];
  char *fieldout;
  sac_header *hdr_in;

  UNUSED(lenvalue);

  hdr_in = indata->ext_hdrs[*hdr_index-1];

  memset(infield,' ',8);
  infield[8]='\0';

  getfield(fieldname, lenfield, infield);

  fieldout = getahdr( hdr_in, infield, error );

  if( *error == 0 ){
    strcpy(value, fieldout);
    value[strlen(value)] = ' ';
  }
  
  return;

}
