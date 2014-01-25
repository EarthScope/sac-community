#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SAC_CHAR_NAME_FIELDS
#include "extfunc.h"
#define MIN(n1,n2) ((n1) < (n2) ? (n1) : (n2))

void setahdr( header_in, fieldname, value, error )
sac_header *header_in;
char      *fieldname;
char          *value;
int          *error;

{
  int i, nchars;
  char longfield[18];
  char shortfield[9]; 

  *error = 0;

  memset(longfield,' ',17);
  longfield[17]='\0';
  memset(shortfield,' ',8); 
  shortfield[8]='\0';

  for( i=0; i<MKHDR; i++){
    if(!strcmp(fieldname,char_hdr_fields[i]))break;
  }

  if( i<MKHDR ) {
    nchars = strlen(value);
    if( i == 1 ) { /* kevnm is 18 chars long */
      strncpy(longfield,value,MIN(nchars,17));
      strcpy(header_in->ext_khdr[i],longfield);
    }else{        /* other fields are 9 chars long */
      strncpy(shortfield,value,MIN(nchars,8));
      strcpy(header_in->ext_khdr[i],shortfield);
    }
  }
  else {
    *error = 1;
    return ;
  }

}
