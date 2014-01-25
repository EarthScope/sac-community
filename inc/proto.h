
#include "isnan.h"

#define ENDIAN_BIG       1
#define ENDIAN_LITTLE    0
#define ENDIAN_UNKNOWN  -1

#define SAC_USE_DATABASE         "SAC_USE_DATABASE"
#define SAC_DISPLAY_COPYRIGHT    "SAC_DISPLAY_COPYRIGHT"
#define SAC_PPK_LARGE_CROSSHAIRS "SAC_PPK_LARGE_CROSSHAIRS"

#define maxfi(f1,f2) (int) fmax(f1,f2)
#define minfi(f1,f2) (int) fmin(f1,f2)

#ifndef MIN
#define MIN(f1,f2) ((f1) < (f2) ? (f1) : (f2))
#endif

