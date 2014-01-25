#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAC_LONG_NAME_FIELDS
#include "extfunc.h"


int getnhdr( header_in, fieldname, error )
sac_header *header_in;
char      *fieldname;
int          *error;

{
  int i;

  *error = 0;

  for( i=0; i<MNHDR; i++){
    if(!strcmp(fieldname,long_hdr_fields[i]))break;
  }

  if( i<MNHDR ) return(header_in->ext_nhdr[i]);
  else {
    *error = 1;
    return (NUNDEF);
  }

}
