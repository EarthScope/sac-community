
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sac.h"
#include "sacio.h"

#define NMAX 10000

void
test_bp() {
    float y[NMAX], b, dt;
    int n, nerr, nmax = NMAX;
    rsac1("raw.sac", y, &n, &b, &dt, &nmax, &nerr, -1);
    bandpass(y, n, dt, 0.10, 1.00);
    sac_compare("bandpass_sac.sac", y, n, b, dt);
    wsac0("bandpass.sac", y, y, &nerr, -1);
}
void
test_lp() {
    float y[NMAX], b, dt;
    int n, nerr, nmax = NMAX;
    rsac1("raw.sac", y, &n, &b, &dt, &nmax, &nerr, -1);
    lowpass(y, n, dt, 2.0);
    sac_compare("lowpass_sac.sac", y, n, b, dt);
    wsac0("lowpass.sac", y, y, &nerr, -1);
}
void
test_hp() {
    float y[NMAX], b, dt;
    int n, nerr, nmax = NMAX;
    rsac1("raw.sac", y, &n, &b, &dt, &nmax, &nerr, -1);
    highpass(y, n, dt, 10.0);
    sac_compare("highpass_sac.sac", y, n, b, dt);
    wsac0("highpass.sac", y, y, &nerr, -1);
}

void
test_filter() {
    float y[NMAX], b, dt;
    int n, nerr, nmax = NMAX;
    rsac1("raw.sac", y, &n, &b, &dt, &nmax, &nerr, -1);
    filter(SAC_BESSEL, SAC_BANDREJECT, y, n, dt, 2.0, 10.00, 2, 4, 0.0, 0.0);
    sac_compare("bandreject_sac.sac", y, n, b, dt);
    wsac0("bandreject.sac", y, y, &nerr, -1);
}

int
main(int argc, char *argv[]) {
    test_bp();
    test_hp();
    test_lp();
    test_filter();
    return 0;
}
