
#include <sac.h>

int
main() {

    #define NMAX  10000
    float y[NMAX], b, dt;
    int n, nerr, nmax = NMAX;

    rsac1("raw.sac", y, &n, &b, &dt, &nmax, &nerr, -1);

    int_trap(y, n, (double)dt);

    n = n - 1;
    setnhv("npts", &n, &nerr, -1);
    b = b + 0.5 *dt;
    setfhv("b", &b, &nerr, -1);
    setihv("idep", "iunkn", &nerr, -1, -1);

    wsac0("int.sac", y, y, &nerr, -1);

    if(sac_compare_to_file("int_sac.sac", y, 1e-4, 0, 0)) {
        exit(1);
    }

    return 0;
}
