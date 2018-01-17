
#include <sac.h>

int
main() {

    #define NMAX  10000
    float y[NMAX], yout[NMAX], b, dt;
    int n, nerr, nmax = NMAX;

    rsac1("raw.sac", y, &n, &b, &dt, &nmax, &nerr, -1);

    dif2(y, n, (double)dt, yout);

    wsac0("dif.sac", y, y, &nerr, -1);

    sac_compare("dif_sac.sac", yout, n-1, b + 0.5 * dt, dt);

    return 0;
}
