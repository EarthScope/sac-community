
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

    wsac0("rtrend.sac", y, y, &nerr, -1);

    sac_compare("rtrend_sac.sac", y, n, b, dt);

    return 0;
}
