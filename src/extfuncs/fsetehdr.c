#include <string.h>

#include "extfunc.h"

extern sac_files *indata;

void
fsetehdr_(int *hdr_index, char *fieldname, int *value, int *error,
          int lenfield)
{
    char infield[9];
    sac_header *hdr_in;

    hdr_in = indata->ext_hdrs[*hdr_index - 1];

    memset(infield, ' ', 8);
    infield[8] = '\0';

    getfield(fieldname, lenfield, infield);

    setehdr(hdr_in, infield, *value, error);

    return;

}
