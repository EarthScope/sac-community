
#include <sac.h>

int
main() {

    #define NMAX  10000
    float y[NMAX], yout[NMAX], b, dt;
    int n, nerr, nmax = NMAX;

    rsac1("raw.sac", y, &n, &b, &dt, &nmax, &nerr, -1);

    dif2(y, n, (double)dt, yout);

    b = b + 0.5 * dt;
    setfhv("b", &b, &nerr, -1);
    setihv("idep", "iunkn", &nerr, -1, -1);
    n = n - 1;
    setnhv("npts", &n, &nerr, -1);
    
    wsac0("difc.sac", yout, yout, &nerr, -1);

    if(sac_compare_to_file("dif_sac.sac", yout, 1e-4, 0, 0)) {
        exit(1);
    }

    return 0;
}
