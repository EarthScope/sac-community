
#include <sac.h>

int
main() {

    #define NMAX  10000
    float y[NMAX], b, dt;
    int n, nerr, nmax = NMAX;

    rsac1("raw.sac", y, &n, &b, &dt, &nmax, &nerr, -1);

    remove_mean(y, n);

    wsac0("rmeanc.sac", y, y, &nerr, -1);

    if(sac_compare_to_file("rmean_sac.sac", y, 1e-4, 0, 0)) {
        exit(1);
    }

    return 0;
}
