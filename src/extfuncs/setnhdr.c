#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAC_LONG_NAME_FIELDS
#include "extfunc.h"


void setnhdr( header_in, fieldname, value, error )
sac_header *header_in;
char      *fieldname;
int           value;
int          *error;

{
  int i;

  *error = 0;

  for( i=0; i<MNHDR; i++){
    if(!strcmp(fieldname,long_hdr_fields[i]))break;
  }

  if( i<MNHDR ) header_in->ext_nhdr[i] = value;
  else *error = 1;

  return;


}
