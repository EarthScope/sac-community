#ifndef SUDS_CONVERT_H
#define SUDS_CONVERT_H

#include "suds.h"

void Convert2(void *in);
void Convert4(void *in);
void Convert8(void *in);

void ConvertPZArray(SUDS_CALIBR *PZA, int npts);
#endif
