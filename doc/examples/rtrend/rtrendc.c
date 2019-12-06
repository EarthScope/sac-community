
#include <stdio.h>
#include "sac.h"

int
main() {

    #define NMAX 10000

    float y[NMAX], b, dt;
    int nmax = NMAX;
    int n, nerr;

    rsac1("raw.sac", y, &n, &b, &dt, &nmax, &nerr, -1);

    remove_trend(y, n, dt, b);

    wsac0("rtrendc.sac", y, y, &nerr, -1);

    if(sac_compare_to_file("rtrend_sac.sac", y, 1e-4, 0, 0)) {
        exit(1);
    }

    return 0;
}
