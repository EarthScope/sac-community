
#include "xyz.h"

void
window_data(float *array, int nxsize, int nysize, float *wdata, int jxstart,
            int jxstop, int jystart, int jystop)
{

    int i, j;
    for (i = jystart - 1; i < jystop; i++) {
        for (j = jxstart - 1; j < jxstop; j++) {
            if (i > 0 && i < nysize && j > 0 && j < nxsize) {
                *(wdata++) = *(array + (i * nxsize) + j);
            } else {
                *(wdata++) = 0.0;
            }
        }
    }

    return;
}
