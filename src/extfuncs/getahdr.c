#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAC_CHAR_NAME_FIELDS
#include "extfunc.h"

char *getahdr( header_in, fieldname, error )
sac_header *header_in;
char      *fieldname;
int          *error;

{
  int i;

  *error = 0;

  for( i=0; i<MKHDR; i++){
    if(!strcmp(fieldname,char_hdr_fields[i]))break;
  }

  if( i<MKHDR ) return(&(header_in->ext_khdr[i][0]));
  else {
    *error = 1;
    return ((char *)NULL);
  }

}
