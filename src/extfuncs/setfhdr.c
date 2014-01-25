#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAC_FLOAT_NAME_FIELDS
#include "extfunc.h"


void setfhdr( header_in, fieldname, value, error )
sac_header *header_in;
char      *fieldname;
float          value;
int          *error;

{
  int i;

  *error = 0;

  for( i=0; i<MFHDR; i++){
    if(!strcmp(fieldname,float_hdr_fields[i]))break;
  }

  if( i<MFHDR ) header_in->ext_fhdr[i] = value;
  else *error = 1;

  return;


}
