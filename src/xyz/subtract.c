
#include "xyz.h"

void
subtract(float *input1, float *input2, float *output, int npoints,
          int *nerr)
{
    int i;

    *nerr = 0;

    for (i = 0; i < npoints; i++) {
        *(output++) = *(input1++) - *(input2++);
    }

    return;

}
