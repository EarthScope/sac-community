
#include <sac.h>

int
main() {

    #define NMAX  10000
    float y[NMAX], b, dt;
    int n, nerr, nmax = NMAX;

    rsac1("raw.sac", y, &n, &b, &dt, &nmax, &nerr, -1);

    remove_mean(y, n);

    wsac0("rmean.sac", y, y, &nerr, -1);

    sac_compare("rmean_sac.sac", y, n, b, dt);

    return 0;
}
