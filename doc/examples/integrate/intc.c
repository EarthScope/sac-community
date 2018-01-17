
#include <sac.h>

int
main() {

    #define NMAX  10000
    float y[NMAX], b, dt;
    int n, nerr, nmax = NMAX;

    rsac1("raw.sac", y, &n, &b, &dt, &nmax, &nerr, -1);

    int_trap(y, n, (double)dt);

    wsac0("int.sac", y, y, &nerr, -1);

    sac_compare("int_sac.sac", y, n-1, b + 0.5 * dt, dt);

    return 0;
}
