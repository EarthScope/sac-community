#include <string.h>

#include "extfunc.h"

extern sac_files *indata;

void
fsetahdr_(hdr_index, fieldname, value, error, lenfield, lenvalue)
     int *hdr_index;
     char *fieldname;
     char *value;
     int *error;
     int lenfield;
     int lenvalue;
{
    char infield[9];
    char invalue[18];
    sac_header *hdr_in;

    hdr_in = indata->ext_hdrs[*hdr_index - 1];

    memset(infield, ' ', 8);
    infield[8] = '\0';
    memset(invalue, ' ', 17);
    invalue[17] = '\0';

    getfield(fieldname, lenfield, infield);
    getfield(value, lenvalue, invalue);

    setahdr(hdr_in, infield, invalue, error);

    return;

}
